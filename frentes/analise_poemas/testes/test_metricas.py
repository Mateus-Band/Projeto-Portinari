import pandas as pd

from src import metricas as mt
from src.stopwords import carregar_stopwords

TOKENS = pd.DataFrame({
    "poema_id": ["p1"] * 5 + ["p2"] * 3,
    "verso_idx": 0, "token_idx": [0, 1, 2, 3, 4, 0, 1, 2],
    "forma": ["as", "águas", "do", "rio", "correm", "a", "água", "dela"],
    "lema": ["o", "água", "de o", "rio", "correr", "o", "água", "de ela"],
    "upos": ["DET", "NOUN", "ADP", "NOUN", "VERB", "DET", "NOUN", "ADP"],
    "morph": ["Gender=Fem|Number=Plur", "Gender=Fem|Number=Plur", "", "Gender=Masc|Number=Sing", "Number=Plur|Tense=Pres",
              "Gender=Fem|Number=Sing", "Gender=Fem|Number=Sing", ""],
    "modelo": "falso", "contexto": "poema",
})
CORPUS = pd.DataFrame({"poema_id": ["p1", "p2", "p3"], "texto": ["As águas / do rio correm.", "A água dela", ""]})


def test_unidades_com_lemas_separam_contracoes_e_marcam_a_componente_principal():
    u = mt.tabela_de_unidades(TOKENS, True)
    assert len(u) == 8 + 2                                                  # "de o" e "de ela" viram 2
    do = u[(u.poema_id == "p1") & (u.token_idx == 2)]
    assert do["unidade"].tolist() == ["de", "o"] and do["primeira"].tolist() == [True, False]
    assert len(mt.tabela_de_unidades(TOKENS, False)) == 8


def test_lema_ausente_cai_para_a_forma():
    t = TOKENS.copy()
    t["lema"] = ""
    assert mt.tabela_de_unidades(t, True)["unidade"].tolist() == t["forma"].tolist()


def test_frequencias_freq_e_df():
    tab = mt.tabela_frequencias(mt.tabela_de_unidades(TOKENS, True))
    agua = tab[tab.palavra == "água"].iloc[0]
    assert agua["freq"] == 2 and agua["df"] == 2 and tab.iloc[0]["rank"] == 1
    sem = mt.tabela_frequencias(mt.tabela_de_unidades(TOKENS, True), carregar_stopwords())
    assert "de" not in set(sem.palavra) and "o" not in set(sem.palavra)


def test_frequencia_por_classe_conta_so_a_componente_principal():
    u = mt.tabela_de_unidades(TOKENS, True)
    por = mt.frequencia_por_classe(u, ["ADP", "DET", "NOUN"])
    assert set(por["ADP"].palavra) == {"de"}            # "o" (2ª componente) não conta como ADP
    assert "o" in set(por["DET"].palavra) and por["NOUN"].iloc[0]["palavra"] == "água"


def test_metricas_por_poema_usam_formas_e_aceitam_poema_vazio():
    m = mt.metricas_por_poema(TOKENS, CORPUS, carregar_stopwords()).set_index("poema_id")
    assert m.loc["p1", "n_tokens"] == 5 and m.loc["p1", "n_sentencas"] == 1 and m.loc["p1", "n_caracteres"] == len("As águas do rio correm.")
    assert m.loc["p3", "n_tokens"] == 0 and pd.isna(m.loc["p3", "comp_medio_palavra"])


def test_hapax():
    h = mt.resumo_hapax(mt.tabela_frequencias(mt.tabela_de_unidades(TOKENS, False)))
    assert h["n_tipos"] == 8 and h["n_hapax"] == 8 and h["prop_hapax_vocabulario"] == 1.0   # as 8 formas são distintas
    h2 = mt.resumo_hapax(mt.tabela_frequencias(mt.tabela_de_unidades(TOKENS, True)))
    assert h2["n_hapax"] < h2["n_tipos"]                                                    # "água", "o" e "de" repetem


def test_distribuicao_upos_exclui_pontuacao_e_simbolos():
    t = pd.concat([TOKENS, TOKENS.iloc[[0]].assign(upos="PUNCT")])
    d = mt.distribuicao_upos(t)
    assert "PUNCT" not in set(d.upos) and abs(d["prop"].sum() - 1) < 1e-9


def test_tracos_e_tabela_classe_traco_valor_com_minimo_de_ocorrencias():
    m = mt.tracos_explodidos(TOKENS)
    assert {"feature", "valor", "traco"} <= set(m.columns) and (m.traco == m.feature + "=" + m.valor).all()
    cheia = mt.tabela_classe_traco_valor(m, 1)
    filtrada = mt.tabela_classe_traco_valor(m, 2)
    assert len(filtrada) < len(cheia)
    g = cheia[(cheia.upos == "NOUN") & (cheia.feature == "Gender")]
    assert abs(g["prop_na_classe"].sum() - 1) < 1e-9


def test_comparar_formas_lemas_vocabulario_diminui():
    resumo, ranking = mt.comparar_formas_lemas(TOKENS, carregar_stopwords(), top=5)
    todas = resumo.set_index("conjunto").loc["todas as palavras"]
    assert todas["tipos_formas"] == 8 and todas["tipos_lemas"] == 6
    assert "variacao_do_posto" in ranking.columns
