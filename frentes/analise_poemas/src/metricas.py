"""Métricas, frequências e contagens morfológicas, a partir dos CSVs de tokens.

Só pandas e numpy: nenhuma biblioteca de PLN é importada aqui.
"""
from __future__ import annotations

import numpy as np
import pandas as pd

from .preprocessamento import SENT_RE, texto_do_poema

CLASSES_PRINCIPAIS = ["NOUN", "VERB", "ADJ", "ADV"]
CLASSES_FORA_DA_MORFOLOGIA = ["PUNCT", "SYM"]

NOMES_CLASSES = {  # associa cada etiqueta UPOS (Universal Part-of-Speech) a um nome em português.
    "NOUN": "substantivos", "VERB": "verbos", "ADJ": "adjetivos", "ADV": "advérbios",
    "PROPN": "nomes próprios", "PRON": "pronomes", "DET": "determinantes", "ADP": "preposições",
    "AUX": "verbos auxiliares", "CCONJ": "conjunções coordenativas", "SCONJ": "conjunções subordinativas",
    "NUM": "numerais", "INTJ": "interjeições", "PART": "partículas", "X": "outros",
}

# Define quais traços morfológicos são relevantes para cada classe gramatical.
FOCOS_MORF = [
    ("VERB", ["VerbForm", "Tense", "Mood", "Person", "Number"]),
    ("AUX", ["VerbForm", "Tense", "Mood", "Person", "Number"]),
    ("NOUN", ["Gender", "Number"]),
    ("ADJ", ["Gender", "Number", "Degree"]),
    ("PRON", ["Person", "Number", "PronType"]),
    ("DET", ["Gender", "Number", "Definite"]),
]


# ----------------------------------------------------------------------------- lexical
def tabela_de_unidades(tokens: pd.DataFrame, usar_lemas: bool) -> pd.DataFrame:
    """Uma linha por unidade contada: poema_id, token_idx, upos, unidade, primeira.

    Com lemas, o lema de uma contração vem com espaço ("em o") e é separado nas componentes antes de
    contar; `primeira` marca a componente que carrega a classe do token (a primeira). Tokens sem lema
    caem para a forma. Sem lemas, a unidade é a forma.
    """
    base = tokens[["poema_id", "token_idx", "upos"]].copy()
    if usar_lemas:
        unidade = tokens["lema"].where(tokens["lema"] != "", tokens["forma"])
        base["unidade"] = unidade.str.split()
        base = base.explode("unidade")
        base = base[base["unidade"].notna() & (base["unidade"] != "")]
        base["primeira"] = ~base.duplicated(["poema_id", "token_idx"])
    else:
        base["unidade"] = tokens["forma"]
        base["primeira"] = True
    return base.reset_index(drop=True)


# retorna uma tabela com palavra, frequência, df (quantidade de diferentes poemas em que a palavra aparece)
# em ordem decrescente de frequência e df
def tabela_frequencias(unidades: pd.DataFrame, stop=None) -> pd.DataFrame:
    """freq = ocorrências totais; df = nº de poemas em que a unidade aparece."""
    u = unidades if not stop else unidades[~unidades["unidade"].isin(stop)]
    if u.empty:
        return pd.DataFrame({"palavra": [], "freq": [], "df": [], "rank": []})
    tab = (
        u.groupby("unidade")
        .agg(freq=("unidade", "size"), df=("poema_id", "nunique"))
        .reset_index()
        .rename(columns={"unidade": "palavra"})
    )
    tab = tab.sort_values(["freq", "df", "palavra"], ascending=[False, False, True]).reset_index(drop=True)
    tab["rank"] = tab.index + 1
    return tab


def metricas_por_poema(tokens: pd.DataFrame, corpus: pd.DataFrame, stop) -> pd.DataFrame:
    """Uma linha por poema, SEMPRE sobre as formas (independe de LEMAS).

    O número de caracteres e de sentenças vem do texto do poema (versos unidos por um espaço);
    as sentenças são separadas por '.', '!', '?' e '…'.
    """
    formas = {pid: g["forma"].tolist() for pid, g in tokens.groupby("poema_id", sort=False)}
    linhas = []
    for pid, bruto in zip(corpus["poema_id"], corpus["texto"]):
        texto = texto_do_poema(bruto)
        toks = formas.get(pid, [])
        n = len(toks)
        sentencas = [s for s in SENT_RE.split(texto) if s.strip()]
        conteudo = [t for t in toks if t not in stop]
        linhas.append({
            "poema_id": pid,
            "n_caracteres": len(texto),                    # número de caracteres
            "n_tokens": n,                                 # número de palavras
            "n_tipos": len(set(toks)),                     # número de palavras diferentes
            "comp_medio_palavra": np.mean([len(t) for t in toks]) if n else np.nan,
            "n_sentencas": len(sentencas),
            "tokens_por_sentenca": n / len(sentencas) if sentencas else np.nan,
            "n_tokens_conteudo": len(conteudo),
            "prop_funcionais": 1 - len(conteudo) / n if n else np.nan,     # proporção de stopwords
        })
    return pd.DataFrame(linhas)


def frequencia_por_classe(unidades: pd.DataFrame, classes, stop=None) -> dict[str, pd.DataFrame]:
    """Tabela de frequências de cada classe. Só a componente principal de cada token conta
    (`primeira`): numa contração como "dela" = "de" + "ela", só "de" conta, na classe ADP."""
    u = unidades[unidades["primeira"]]
    if stop:
        u = u[~u["unidade"].isin(stop)]
    return {c: tabela_frequencias(u[u["upos"] == c]) for c in classes if (u["upos"] == c).any()}


def resumo_hapax(freq: pd.DataFrame) -> dict:
    """Hapax = unidade que ocorre uma única vez no corpus."""
    n_tipos, n_tokens = len(freq), int(freq["freq"].sum()) if len(freq) else 0
    n_hapax = int((freq["freq"] == 1).sum()) if len(freq) else 0
    return {
        "n_tokens": n_tokens,
        "n_tipos": n_tipos,
        "n_hapax": n_hapax,
        "prop_hapax_vocabulario": n_hapax / n_tipos if n_tipos else np.nan,
        "prop_hapax_tokens": n_hapax / n_tokens if n_tokens else np.nan,
    }


def comparar_formas_lemas(tokens: pd.DataFrame, stop, top: int = 50):
    """Tamanho do vocabulário com formas e com lemas, e variação do ranking dos `top` lemas de conteúdo.
    Devolve (resumo, ranking)."""
    f, l = tabela_de_unidades(tokens, False), tabela_de_unidades(tokens, True)
    f_todas, l_todas = tabela_frequencias(f), tabela_frequencias(l)
    f_cont, l_cont = tabela_frequencias(f, stop), tabela_frequencias(l, stop)
    resumo = pd.DataFrame([
        {"conjunto": "todas as palavras", "tipos_formas": len(f_todas), "tipos_lemas": len(l_todas)},
        {"conjunto": "palavras de conteúdo", "tipos_formas": len(f_cont), "tipos_lemas": len(l_cont)},
    ])
    resumo["reducao_do_vocabulario"] = 1 - resumo["tipos_lemas"] / resumo["tipos_formas"].where(resumo["tipos_formas"] > 0)
    r_forma = f_cont.set_index("palavra")
    ranking = l_cont.head(top).rename(columns={"palavra": "lema", "freq": "freq_lema", "df": "df_lema", "rank": "rank_lema"})
    ranking["freq_forma_igual"] = ranking["lema"].map(r_forma["freq"]) if len(r_forma) else np.nan
    ranking["rank_forma_igual"] = ranking["lema"].map(r_forma["rank"]) if len(r_forma) else np.nan
    ranking["variacao_do_posto"] = ranking["rank_forma_igual"] - ranking["rank_lema"]   # > 0: sobe ao lematizar
    return resumo, ranking.reset_index(drop=True)


# ----------------------------------------------------------------------------- morfológica
def distribuicao_upos(tokens: pd.DataFrame) -> pd.DataFrame:
    """Contagem e proporção de cada classe gramatical, excluindo PUNCT e SYM."""
    t = tokens[(tokens["upos"] != "") & ~tokens["upos"].isin(CLASSES_FORA_DA_MORFOLOGIA)]
    n = t["upos"].value_counts()
    return pd.DataFrame({"upos": n.index, "n": n.to_numpy(), "prop": n.to_numpy() / max(n.sum(), 1)})


def tracos_explodidos(tokens: pd.DataFrame) -> pd.DataFrame:
    """Uma linha por (ocorrência, traço): poema_id, token_idx, forma, upos, traco ('Number=Sing'),
    feature ('Number') e valor ('Sing'). A unidade é a ocorrência do token, não o lema."""
    t = tokens[(tokens["morph"] != "") & ~tokens["upos"].isin(CLASSES_FORA_DA_MORFOLOGIA)]
    m = t.assign(traco=t["morph"].str.split("|")).explode("traco")
    m = m[m["traco"].notna() & (m["traco"] != "")].copy()
    partes = m["traco"].str.split("=", n=1, expand=True).reindex(columns=[0, 1])
    m["feature"], m["valor"] = partes[0], partes[1]
    return m[["poema_id", "token_idx", "forma", "upos", "traco", "feature", "valor"]].reset_index(drop=True)


def tabela_classe_traco_valor(m: pd.DataFrame, min_ocorrencias: int = 10) -> pd.DataFrame:
    """upos, feature, valor, n, prop_na_classe. A proporção é calculada ANTES de descartar as
    combinações com menos de `min_ocorrencias` (por isso as proporções da tabela filtrada podem somar < 1)."""
    t = m.groupby(["upos", "feature", "valor"]).size().rename("n").reset_index()
    t["prop_na_classe"] = t["n"] / t.groupby(["upos", "feature"])["n"].transform("sum")
    t = t[t["n"] >= min_ocorrencias]
    return t.sort_values(["upos", "feature", "n", "valor"], ascending=[True, True, False, True]).reset_index(drop=True)


def top_tracos(m: pd.DataFrame, n_palavras: int) -> pd.DataFrame:
    """Traços agregados `Feature=Valor`. ATENÇÃO: a contagem mistura classes (um Number=Sing de
    substantivo e um de verbo contam igual). `prop_palavras` = fração das palavras com o traço."""
    c = m["traco"].value_counts()
    return pd.DataFrame({"traco": c.index, "n": c.to_numpy(), "prop_palavras": c.to_numpy() / max(n_palavras, 1)})
