import hashlib

import pandas as pd
import pytest

from src.carga import carregar_corpus, carregar_tokens
from src.tokenizacao import CONTEXTOS, conferir_consistencia, gerar_tokens
from testes.modelo_falso import ModeloFalso

POEMAS = pd.DataFrame({
    "poema_id": ["p1", "p2", "p3"],
    "texto": [
        "O rio do povo / Corria devagar. / Emily cantava pela / casa dela, / bem-estar e d'água.",
        "A lua / olhava o menino / Dele",
        "",
    ],
})


def tokens_por_contexto(tmp_path, **kw):
    modelo = ModeloFalso(**kw)
    return {c: gerar_tokens(POEMAS, modelo, c, pasta_analise=tmp_path) for c in CONTEXTOS}


def test_os_tres_contextos_geram_csvs_distintos_com_os_mesmos_tokens(tmp_path):
    res = tokens_por_contexto(tmp_path)
    arquivos = sorted(p.name for p in tmp_path.glob("tokens_*.csv"))
    assert arquivos == ["tokens_poema_falso.csv", "tokens_token_falso.csv", "tokens_verso_falso.csv"]
    chaves = {c: r.tokens[["poema_id", "verso_idx", "token_idx", "forma"]].to_numpy().tolist() for c, r in res.items()}
    assert chaves["poema"] == chaves["verso"] == chaves["token"]
    assert conferir_consistencia(tmp_path)["mesmos_tokens"].all()


def test_contracoes_viram_um_token_com_lema_em_componentes(tmp_path):
    t = tokens_por_contexto(tmp_path)["poema"].tokens
    do = t[(t.poema_id == "p1") & (t.forma == "do")].iloc[0]
    assert do.lema == "de o" and do.upos == "ADP"
    assert t[t.forma == "dela"].iloc[0].lema == "de ela"
    assert tokens_por_contexto(tmp_path)["poema"].registro["n_reagrupados"] >= 3


def test_hifen_e_apostrofo_sao_um_token_mesmo_se_o_modelo_parte_a_palavra(tmp_path):
    res = {c: gerar_tokens(POEMAS, ModeloFalso(separar_hifen=True), c, pasta_analise=tmp_path, salvar=False) for c in CONTEXTOS}
    for r in res.values():
        t = r.tokens
        assert {"bem-estar", "d'água"} <= set(t.forma)
        assert len(r.divergencias) == 0
    assert res["poema"].tokens[res["poema"].tokens.forma == "bem-estar"].iloc[0].lema == "bem estar"


def test_cada_contexto_entrega_ao_modelo_a_unidade_certa(tmp_path):
    m = ModeloFalso()
    gerar_tokens(POEMAS, m, "poema", pasta_analise=tmp_path, salvar=False)
    assert len(m.chamadas[-1]) == 2 and "\n" not in m.chamadas[-1][0]            # só poemas não vazios
    m = ModeloFalso()
    gerar_tokens(POEMAS, m, "verso", pasta_analise=tmp_path, salvar=False)
    assert len(m.chamadas[-1]) == 8                                              # 5 + 3 versos
    m = ModeloFalso()
    gerar_tokens(POEMAS, m, "token", pasta_analise=tmp_path, salvar=False)
    assert all(" " not in t for t in m.chamadas[-1]) and len(set(m.chamadas[-1])) == len(m.chamadas[-1])


def test_normalizacao_chega_ao_modelo_nos_tres_contextos(tmp_path):
    for contexto in CONTEXTOS:
        com, sem = ModeloFalso(), ModeloFalso()
        gerar_tokens(POEMAS, com, contexto, normalizar=True, pasta_analise=tmp_path, salvar=False)
        gerar_tokens(POEMAS, sem, contexto, normalizar=False, pasta_analise=tmp_path, salvar=False)
        entregue_com = " ".join(com.chamadas[-1])
        entregue_sem = " ".join(sem.chamadas[-1])
        assert "Corria" not in entregue_com and "corria" in entregue_com, contexto
        assert "Corria" in entregue_sem, contexto
        assert "Emily" in entregue_com, contexto            # abre frase (verso anterior termina em '.'): preservado


def test_modelo_sem_lema_ou_sem_tracos_deixa_colunas_vazias(tmp_path):
    t = gerar_tokens(POEMAS, ModeloFalso(fornece_lema=False, fornece_morph=False), "poema", pasta_analise=tmp_path).tokens
    assert (t["lema"] == "").all() and (t["morph"] == "").all() and (t["upos"] != "").any()
    assert not (tmp_path / "lemas_poema_falso.csv").exists()
    lido = carregar_tokens("falso", "poema", tmp_path)
    assert (lido["lema"] == "").all()


def test_executar_duas_vezes_gera_arquivos_identicos(tmp_path):
    def hashes():
        tokens_por_contexto(tmp_path)
        return {p.name: hashlib.md5(p.read_bytes()).hexdigest() for p in sorted(tmp_path.glob("*.csv")) if p.name != "registro.csv"}
    primeiro = hashes()
    assert primeiro == hashes()
    assert len(pd.read_csv(tmp_path / "registro.csv")) == 6        # uma linha por execução e contexto


def test_registro_tem_modelo_versao_acesso_contexto_e_contagens(tmp_path):
    r = gerar_tokens(POEMAS, ModeloFalso(), "verso", normalizar=False, pasta_analise=tmp_path).registro
    reg = pd.read_csv(tmp_path / "registro.csv").iloc[0]
    assert reg["modelo"] == "falso" and reg["versao"] == "modelo falso 1.0" and reg["acesso"] == "local"
    assert reg["contexto"] == "verso" and reg["normalizar_maiusculas"] == False       # noqa: E712
    assert reg["n_poemas"] == 3 and reg["n_tokens"] == r["n_tokens"] > 0 and reg["n_divergencias"] == 0


def test_contexto_invalido():
    with pytest.raises(ValueError):
        gerar_tokens(POEMAS, ModeloFalso(), "frase", salvar=False)


def test_carregar_tokens_ausente_pede_para_rodar_o_notebook_01(tmp_path):
    with pytest.raises(FileNotFoundError, match="01_tokenizacao"):
        carregar_tokens("spacy", "poema", tmp_path)


def test_carregar_corpus_limpa_gera_ids_e_recusa_repetidos(tmp_path):
    arq = tmp_path / "c.csv"
    arq.write_text('estrofes,ano\n"  a   b / c ",1950\n,1951\n', encoding="utf-8")
    c = carregar_corpus(arq)
    assert c["poema_id"].tolist() == ["poema_001", "poema_002"] and c["texto"].tolist() == ["a b / c", ""]
    arq.write_text('id,estrofes\nx,a\nx,b\n', encoding="utf-8")
    with pytest.raises(ValueError):
        carregar_corpus(arq)


def test_modelos_e_contextos_consistentes_entre_modulos():
    from src import carga
    from src.modelos.base import CAPACIDADES
    assert tuple(CAPACIDADES) == carga.MODELOS
    assert CONTEXTOS == carga.CONTEXTOS
    with pytest.raises(ValueError):
        carga.validar_parametros("gpt", "poema")
    with pytest.raises(ValueError):
        carga.validar_parametros("spacy", "frase")
    carga.validar_parametros("spacy", "verso")
