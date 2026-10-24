"""Leitura e limpeza do CSV de poemas e leitura dos CSVs de tokens gerados pelo notebook 01."""
from __future__ import annotations

from pathlib import Path

import pandas as pd

RAIZ = Path(__file__).resolve().parents[1] #sobe dois níveis do arquivo atual
PASTA_BRUTO = RAIZ / "dados" / "bruto"
PASTA_ANALISE = RAIZ / "dados" / "analise"
PASTA_ANOTADO = RAIZ / "dados" / "anotado"
PASTA_SAIDA = RAIZ / "saida"

CONTEXTOS = ("token", "verso", "poema")
MODELOS = ("spacy", "stanza", "udpipe2", "trankit", "bertimbau_ft")

COLUNAS_TOKENS = ["poema_id", "verso_idx", "token_idx", "forma", "lema", "upos", "morph", "modelo", "contexto"]


def carregar_corpus(caminho: str | Path) -> pd.DataFrame:
    """Lê o CSV de poemas. Devolve duas colunas: `poema_id` e `texto` (a coluna `estrofes` limpa,
    ainda com os versos separados por '/').

    Só `estrofes` e `id` são usadas; as demais colunas (metadados) são ignoradas.
    Se não houver `id`, são gerados poema_001, poema_002... Valores vazios em `estrofes` viram
    texto vazio e espaços repetidos são colapsados em um.
    """
    df = pd.read_csv(caminho, encoding="utf-8")
    if "estrofes" not in df.columns:
        raise ValueError(f"Coluna 'estrofes' não encontrada em {caminho}.")
    if "id" in df.columns:
        ids = df["id"].astype(str)
    else:
        ids = pd.Series([f"poema_{i + 1:03d}" for i in range(len(df))], index=df.index)
    repetidos = ids[ids.duplicated()].unique().tolist()
    if repetidos:
        raise ValueError(f"Ids de poema repetidos (o alinhamento dependeria deles): {repetidos[:5]}")
    texto = df["estrofes"].fillna("").astype(str).str.replace(r"\s+", " ", regex=True).str.strip()
    return pd.DataFrame({"poema_id": ids.to_numpy(), "texto": texto.to_numpy()})


def validar_parametros(modelo: str, contexto: str) -> None:
    """Para com uma mensagem clara se MODELO ou CONTEXTO tiverem valores inválidos."""
    if modelo not in MODELOS:
        raise ValueError(f"MODELO deve ser um de {MODELOS}; recebido: {modelo!r}")
    if contexto not in CONTEXTOS:
        raise ValueError(f"CONTEXTO deve ser um de {CONTEXTOS}; recebido: {contexto!r}")


def caminho_tokens(contexto: str, modelo: str, pasta: Path | None = None) -> Path:
    return (pasta or PASTA_ANALISE) / f"tokens_{contexto}_{modelo}.csv"


def caminho_lemas(contexto: str, modelo: str, pasta: Path | None = None) -> Path:
    return (pasta or PASTA_ANALISE) / f"lemas_{contexto}_{modelo}.csv"


def pasta_saida(tipo: str, modelo: str, contexto: str, raiz: Path | None = None, criar: bool = True) -> Path:
    """saida/{lexical|morfologica}/{modelo}_{contexto}/"""
    if tipo not in ("lexical", "morfologica"):
        raise ValueError("tipo deve ser 'lexical' ou 'morfologica'.")
    pasta = (raiz or PASTA_SAIDA) / tipo / f"{modelo}_{contexto}"
    if criar:
        pasta.mkdir(parents=True, exist_ok=True)
    return pasta


def carregar_tokens(modelo: str, contexto: str, pasta: Path | None = None) -> pd.DataFrame:
    """Lê dados/analise/tokens_{contexto}_{modelo}.csv. Se não existir, para com uma mensagem
    pedindo para rodar o notebook 01 com esses valores."""
    arquivo = caminho_tokens(contexto, modelo, pasta)
    if not arquivo.exists():
        raise FileNotFoundError(
            f"Arquivo não encontrado: {arquivo}\n"
            f"Rode antes o notebook 01_tokenizacao com MODELO = '{modelo}' e CONTEXTO = '{contexto}'."
        )
    # keep_default_na=False: colunas vazias (ex.: lema de modelo que não lematiza) ficam ""
    tokens = pd.read_csv(arquivo, encoding="utf-8", dtype=str, keep_default_na=False)
    faltando = [c for c in COLUNAS_TOKENS if c not in tokens.columns]
    if faltando:
        raise ValueError(f"{arquivo} não tem as colunas {faltando}.")
    for col in ("verso_idx", "token_idx"):
        tokens[col] = tokens[col].astype(int)
    return tokens[COLUNAS_TOKENS]


def tem_lema(tokens: pd.DataFrame) -> bool:
    return bool((tokens["lema"] != "").any())


def tem_morph(tokens: pd.DataFrame) -> bool:
    return bool((tokens["morph"] != "").any())


def tem_upos(tokens: pd.DataFrame) -> bool:
    return bool((tokens["upos"] != "").any())
