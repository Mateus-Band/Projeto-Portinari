"""Notebook 01: etiqueta o corpus com um modelo e um contexto e grava os CSVs.

Fluxo: preparar os poemas (versos, normalização de maiúsculas, tokens comuns) -> entregar ao modelo
conforme o CONTEXTO (token, verso ou poema) -> alinhar as palavras do modelo aos tokens comuns ->
gravar tokens_{contexto}_{modelo}.csv, lemas_{contexto}_{modelo}.csv e uma linha em registro.csv.
"""
from __future__ import annotations

from collections import defaultdict
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path

import pandas as pd

from .alinhamento import alinhar
from .carga import COLUNAS_TOKENS, CONTEXTOS, PASTA_ANALISE, caminho_lemas, caminho_tokens
from .modelos.base import UPOS_VALIDOS, Modelo, PalavraModelo
from .preprocessamento import PoemaPreparado, preparar_corpus

COLUNAS_REGISTRO = [
    "data", "modelo", "versao", "acesso", "contexto", "normalizar_maiusculas",
    "n_poemas", "n_tokens", "n_reagrupados", "n_divergencias", "arquivo_tokens",
]


@dataclass
class ResultadoTokenizacao:
    tokens: pd.DataFrame
    lemas: pd.DataFrame | None
    divergencias: pd.DataFrame
    registro: dict


def _deslocar(palavras: list[PalavraModelo], deslocamento: int) -> list[PalavraModelo]:
    return [PalavraModelo(p.ini + deslocamento, p.fim + deslocamento, p.texto, p.lema, p.upos, p.morph)
            for p in palavras]


def analisar_corpus(modelo: Modelo, poemas: list[PoemaPreparado], contexto: str) -> dict[str, list[PalavraModelo]]:
    """Entrega os textos ao modelo conforme o contexto. Devolve, por poema, as palavras do modelo com
    posições relativas ao texto do poema (versos unidos por um espaço)."""
    por_poema: dict[str, list[PalavraModelo]] = defaultdict(list)
    if contexto == "poema":
        com_texto = [p for p in poemas if p.texto]
        for p, palavras in zip(com_texto, modelo.analisar([p.texto for p in com_texto])):
            por_poema[p.poema_id] = palavras
    elif contexto == "verso":
        entradas = [(p.poema_id, ini, v) for p in poemas for ini, v in zip(p.inicios, p.versos)]
        for (pid, ini, _), palavras in zip(entradas, modelo.analisar([v for _, _, v in entradas])):
            por_poema[pid].extend(_deslocar(palavras, ini))
    elif contexto == "token":
        unicos = sorted({t.texto for p in poemas for t in p.tokens})      # cada forma distinta uma vez
        resultado = dict(zip(unicos, modelo.analisar(unicos)))
        for p in poemas:
            for t in p.tokens:
                por_poema[p.poema_id].extend(_deslocar(resultado[t.texto], t.ini))
    else:
        raise ValueError(f"CONTEXTO deve ser um de {CONTEXTOS}; recebido: {contexto!r}")
    return por_poema


def limpar_lema(lema: str) -> str:
    """Lema em minúsculas e sem espaços sobrando; '_' (ausente no CoNLL-U) vira vazio."""
    lema = " ".join(str(lema).split()).lower()
    return "" if lema == "_" else lema


def limpar_morph(morph: str) -> str:
    morph = str(morph).strip()
    return "" if morph == "_" else morph


def validar_upos(upos: str) -> str:
    upos = str(upos).strip().upper()
    if not upos:
        return ""
    return upos if upos in UPOS_VALIDOS else "X"


def _salvar_csv(df: pd.DataFrame, caminho: Path) -> None:
    caminho.parent.mkdir(parents=True, exist_ok=True)
    df.to_csv(caminho, index=False, encoding="utf-8", lineterminator="\n")


def tabela_de_lemas(tokens: pd.DataFrame) -> pd.DataFrame:
    """Correspondência forma -> (classe, lema) com contagem, para auditoria do lematizador
    (formas com mais de um lema, lemas suspeitos, palavras não lematizadas)."""
    if tokens.empty:
        return pd.DataFrame(columns=["forma", "upos", "lema", "n"])
    tabela = tokens.groupby(["forma", "upos", "lema"]).size().reset_index(name="n")
    return tabela.sort_values(["n", "forma", "upos", "lema"], ascending=[False, True, True, True]).reset_index(drop=True)


def gerar_tokens(corpus: pd.DataFrame, modelo: Modelo, contexto: str, normalizar: bool = True,
                 pasta_analise: Path | None = None, salvar: bool = True) -> ResultadoTokenizacao:
    """Etiqueta o corpus. `corpus` tem as colunas poema_id e texto (ver carga.carregar_corpus)."""
    pasta = Path(pasta_analise) if pasta_analise else PASTA_ANALISE
    poemas = preparar_corpus(corpus, normalizar)
    palavras_por_poema = analisar_corpus(modelo, poemas, contexto)

    linhas, divergencias, n_reagrupados = [], [], 0
    for p in poemas:
        r = alinhar(p.tokens, palavras_por_poema.get(p.poema_id, []))
        n_reagrupados += r.n_reagrupados
        divergencias += [{"poema_id": p.poema_id, **d} for d in r.divergencias]
        for t, a in zip(p.tokens, r.linhas):
            linhas.append({
                "poema_id": p.poema_id,
                "verso_idx": t.verso_idx,
                "token_idx": t.idx,
                "forma": t.texto.lower(),
                "lema": limpar_lema(a["lema"]) if modelo.fornece_lema else "",
                "upos": validar_upos(a["upos"]),
                "morph": limpar_morph(a["morph"]) if modelo.fornece_morph else "",
                "modelo": modelo.nome,
                "contexto": contexto,
            })
    tokens = pd.DataFrame(linhas, columns=COLUNAS_TOKENS)
    lemas = tabela_de_lemas(tokens) if modelo.fornece_lema else None
    div = pd.DataFrame(divergencias, columns=["poema_id", "token_idx", "token", "motivo", "palavras_modelo"])

    arquivo_tokens = caminho_tokens(contexto, modelo.nome, pasta)
    registro = {
        "data": datetime.now().isoformat(timespec="seconds"),
        "modelo": modelo.nome,
        "versao": modelo.versao(),
        "acesso": modelo.acesso(),
        "contexto": contexto,
        "normalizar_maiusculas": bool(normalizar),
        "n_poemas": len(poemas),
        "n_tokens": len(tokens),
        "n_reagrupados": n_reagrupados,
        "n_divergencias": len(div),
        "arquivo_tokens": arquivo_tokens.name,
    }
    if salvar:
        _salvar_csv(tokens, arquivo_tokens)
        if lemas is not None:
            _salvar_csv(lemas, caminho_lemas(contexto, modelo.nome, pasta))
        arquivo_div = pasta / f"divergencias_{contexto}_{modelo.nome}.csv"
        if len(div):
            _salvar_csv(div, arquivo_div)
        elif arquivo_div.exists():
            arquivo_div.unlink()                      # não deixar divergências de uma execução anterior
        registrar(registro, pasta)
    return ResultadoTokenizacao(tokens, lemas, div, registro)


def registrar(registro: dict, pasta: Path | None = None) -> Path:
    """Acrescenta uma linha a dados/analise/registro.csv."""
    arquivo = Path(pasta or PASTA_ANALISE) / "registro.csv"
    arquivo.parent.mkdir(parents=True, exist_ok=True)
    existe = arquivo.exists()
    pd.DataFrame([registro], columns=COLUNAS_REGISTRO).to_csv(
        arquivo, mode="a", header=not existe, index=False, encoding="utf-8", lineterminator="\n"
    )
    return arquivo


def conferir_consistencia(pasta: Path | None = None) -> pd.DataFrame:
    """Compara o conjunto de tokens (poema, posição, verso, forma) de todos os tokens_*.csv da pasta.
    Todos os modelos e contextos devem ter exatamente os mesmos tokens."""
    pasta = Path(pasta or PASTA_ANALISE)
    chave = ["poema_id", "token_idx", "verso_idx", "forma"]
    referencia, linhas = None, []
    for arquivo in sorted(pasta.glob("tokens_*.csv")):
        df = pd.read_csv(arquivo, dtype=str, keep_default_na=False)[chave].astype({"token_idx": int, "verso_idx": int})
        conjunto = set(map(tuple, df.to_numpy().tolist()))
        if referencia is None:
            referencia = conjunto
        linhas.append({
            "arquivo": arquivo.name,
            "n_tokens": len(df),
            "mesmos_tokens": conjunto == referencia,
            "so_neste": len(conjunto - referencia),
            "faltando": len(referencia - conjunto),
        })
    return pd.DataFrame(linhas, columns=["arquivo", "n_tokens", "mesmos_tokens", "so_neste", "faltando"])


def comparar_normalizacao(corpus: pd.DataFrame, modelo: Modelo, contextos=CONTEXTOS,
                          n_amostra: int = 15, semente: int = 42):
    """Teste da normalização: para cada contexto, roda com NORMALIZAR_MAIUSCULAS em True e em False e
    compara a proporção de PROPN e as palavras cuja classe mudou. Não grava arquivos.
    Devolve (resumo por contexto, amostra das palavras cuja classe mudou)."""
    resumo, amostras = [], []
    for contexto in contextos:
        com = gerar_tokens(corpus, modelo, contexto, normalizar=True, salvar=False).tokens
        sem = gerar_tokens(corpus, modelo, contexto, normalizar=False, salvar=False).tokens
        m = com.merge(sem, on=["poema_id", "token_idx"], suffixes=("_com", "_sem"))
        mudou = m[m["upos_com"] != m["upos_sem"]]
        n = max(len(m), 1)
        resumo.append({
            "contexto": contexto,
            "n_tokens": len(m),
            "prop_propn_com": (m["upos_com"] == "PROPN").sum() / n,
            "prop_propn_sem": (m["upos_sem"] == "PROPN").sum() / n,
            "n_classe_mudou": len(mudou),
            "prop_classe_mudou": len(mudou) / n,
        })
        if len(mudou):
            a = mudou.sample(min(n_amostra, len(mudou)), random_state=semente).sort_values(["poema_id", "token_idx"])
            amostras.append(pd.DataFrame({
                "contexto": contexto, "poema_id": a["poema_id"], "token_idx": a["token_idx"],
                "forma": a["forma_com"], "upos_com_normalizacao": a["upos_com"], "upos_sem_normalizacao": a["upos_sem"],
            }))
    amostra = pd.concat(amostras, ignore_index=True) if amostras else pd.DataFrame()
    return pd.DataFrame(resumo), amostra
