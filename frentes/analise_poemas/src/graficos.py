"""Funções de figura. Cada uma salva o .png (se houver caminho) e exibe a figura no notebook."""
from __future__ import annotations

import random
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

PALETA_NUVEM = ["#F2389E", "#7660A6", "#049DD9", "#C5D932", "#F2B705"]
AZUL, LARANJA, VERDE = "#4C72B0", "#DD8452", "#55A868"


def finalizar(fig, caminho=None, mostrar=True):
    """Salva a figura (se houver caminho) e a exibe no notebook."""
    fig.tight_layout()
    if caminho is not None:
        fig.savefig(caminho, dpi=150, bbox_inches="tight")
    if mostrar:
        plt.show()
    else:
        plt.close(fig)


def barras_frequencia(tab: pd.DataFrame, titulo: str, caminho=None, top: int = 50, mostrar: bool = True):
    """Barras das `top` palavras: frequência total (azul) e nº de poemas (laranja)."""
    if tab.empty:
        print(f"(sem dados para '{titulo}')")
        return
    t = tab.head(top).iloc[::-1]
    fig, ax = plt.subplots(figsize=(7, 0.28 * len(t) + 1.5))
    ax.barh(t["palavra"], t["freq"], color=AZUL, label="frequência total")
    ax.barh(t["palavra"], t["df"], color=LARANJA, height=0.4, label="nº de poemas (df)")
    ax.set_title(titulo)
    ax.set_xlabel("contagem")
    ax.legend(loc="lower right")
    finalizar(fig, caminho, mostrar)


def nuvem(freq: pd.Series, caminho=None, semente: int = 42, fonte=None, titulo=None,
          max_palavras: int = 150, mostrar: bool = True):
    """Nuvem de palavras (índice = palavra, valor = frequência). Paleta e semente fixas: reprodutível."""
    try:
        from wordcloud import WordCloud
    except ImportError:
        print("wordcloud não instalado (pip install wordcloud): nuvem ignorada.")
        return None
    dados = {str(k): int(v) for k, v in freq.items() if v > 0}
    if not dados:
        return None
    if fonte and not Path(fonte).exists():
        print(f"fonte '{fonte}' não encontrada: usando a fonte padrão da nuvem.")
        fonte = None
    rng = random.Random(semente)

    def cor(*args, **kwargs):
        return rng.choice(PALETA_NUVEM)

    wc = WordCloud(
        width=1400, height=800, background_color="white", max_words=max_palavras, collocations=False,
        font_path=fonte, prefer_horizontal=0.8, random_state=semente, color_func=cor,
    ).generate_from_frequencies(dados)
    if caminho is not None:
        wc.to_file(str(caminho))
    if mostrar:
        fig, ax = plt.subplots(figsize=(12, 7))
        ax.imshow(wc, interpolation="bilinear")
        ax.axis("off")
        if titulo:
            ax.set_title(titulo)
        finalizar(fig, None, True)
    return wc


def painel_classes(tabelas: dict, nomes: dict, titulo: str, caminho=None, mostrar: bool = True, top: int = 10):
    """Painel com o top `top` de cada classe de `tabelas` (classe -> tabela de frequências)."""
    if not tabelas:
        return
    classes = list(tabelas)
    n_col = 2
    n_lin = int(np.ceil(len(classes) / n_col))
    fig, eixos = plt.subplots(n_lin, n_col, figsize=(5.4 * n_col, 0.34 * top * n_lin + 1.6 * n_lin), squeeze=False)
    for ax, c in zip(eixos.ravel(), classes):
        t = tabelas[c].head(top).iloc[::-1]
        ax.barh(t["palavra"], t["freq"], color=AZUL)
        ax.set_title(f"{nomes.get(c, c)} ({c})", fontsize=10)
        ax.tick_params(labelsize=8)
    for ax in eixos.ravel()[len(classes):]:
        ax.axis("off")
    fig.suptitle(titulo)
    finalizar(fig, caminho, mostrar)


def zipf(tab: pd.DataFrame, titulo: str, caminho=None, mostrar: bool = True):
    """Curva de frequência por posto (Zipf), em escala log-log."""
    if tab.empty:
        return
    fig, ax = plt.subplots(figsize=(6, 4.5))
    ax.loglog(tab["rank"], tab["freq"], marker=".", linestyle="none", markersize=3)
    ax.set_xlabel("posto (log)")
    ax.set_ylabel("frequência (log)")
    ax.set_title(titulo)
    ax.grid(True, which="both", alpha=0.3)
    finalizar(fig, caminho, mostrar)


def comparacao_vocabulario(resumo: pd.DataFrame, caminho=None, mostrar: bool = True):
    """Tamanho do vocabulário com formas e com lemas."""
    fig, ax = plt.subplots(figsize=(6.5, 4))
    x = np.arange(len(resumo))
    ax.bar(x - 0.2, resumo["tipos_formas"], width=0.4, color=AZUL, label="formas")
    ax.bar(x + 0.2, resumo["tipos_lemas"], width=0.4, color=LARANJA, label="lemas")
    ax.set_xticks(x)
    ax.set_xticklabels(resumo["conjunto"])
    ax.set_ylabel("nº de tipos distintos")
    ax.set_title("Vocabulário: formas × lemas")
    ax.legend()
    finalizar(fig, caminho, mostrar)


def barras_upos(tab: pd.DataFrame, nomes: dict, caminho=None, mostrar: bool = True):
    """Frequência (nº de palavras) de cada classe gramatical."""
    t = tab.sort_values("n")
    fig, ax = plt.subplots(figsize=(7, 0.35 * len(t) + 1.5))
    ax.barh(t["upos"], t["n"], color=AZUL)
    for y, (n, p) in enumerate(zip(t["n"], t["prop"])):
        ax.text(n, y, f" {p:.1%}", va="center", fontsize=8, color="#555")
    ax.set_xlabel("frequência (nº de palavras na classe)")
    ax.set_title("Classes gramaticais (UPOS)")
    finalizar(fig, caminho, mostrar)


def barras_traco_por_classe(tab: pd.DataFrame, upos: str, features, nome: str, caminho=None, mostrar: bool = True):
    """Um gráfico por traço de interesse da classe `upos`: proporção de cada valor entre as palavras da classe."""
    t = tab[tab["upos"] == upos]
    feats = [f for f in features if (t["feature"] == f).any()]
    if not feats:
        return False
    fig, eixos = plt.subplots(1, len(feats), figsize=(3.6 * len(feats), 3.6), squeeze=False)
    for ax, f in zip(eixos[0], feats):
        s = t[t["feature"] == f].sort_values("n", ascending=False)
        ax.bar(s["valor"], s["prop_na_classe"], color=AZUL)
        for x, (p, n) in enumerate(zip(s["prop_na_classe"], s["n"])):
            ax.text(x, p, f"n={n}", ha="center", va="bottom", fontsize=7)
        ax.set_title(f, fontsize=10)
        ax.set_ylim(0, 1.12)
        ax.tick_params(axis="x", rotation=45, labelsize=8)
    eixos[0][0].set_ylabel("proporção na classe")
    fig.suptitle(f"{nome} ({upos})")
    finalizar(fig, caminho, mostrar)
    return True


def figura_verbos(m: pd.DataFrame, caminho=None, mostrar: bool = True):
    """Distribuição de VerbForm, Tense e Mood nas formas verbais (VERB + AUX)."""
    v = m[m["upos"].isin(["VERB", "AUX"])]
    feats = [f for f in ("VerbForm", "Tense", "Mood") if (v["feature"] == f).any()]
    if not feats:
        return False
    fig, eixos = plt.subplots(1, len(feats), figsize=(4.2 * len(feats), 3.8), squeeze=False)
    for ax, f in zip(eixos[0], feats):
        c = v.loc[v["feature"] == f, "valor"].value_counts()
        ax.bar(c.index, c.to_numpy() / c.sum(), color=VERDE)
        ax.set_title(f"{f} (verbos e auxiliares)")
        ax.set_ylabel("proporção")
        ax.tick_params(axis="x", rotation=45)
    finalizar(fig, caminho, mostrar)
    return True


def barras_tracos(tab: pd.DataFrame, top: int, caminho=None, mostrar: bool = True):
    """Top traços agregados. ATENÇÃO: a contagem mistura classes gramaticais."""
    t = tab.head(top).iloc[::-1]
    fig, ax = plt.subplots(figsize=(8, 0.3 * len(t) + 1.5))
    ax.barh(t["traco"], t["n"], color=AZUL)
    ax.tick_params(axis="y", labelsize=8)
    ax.set_xlabel("nº de palavras com o traço")
    ax.set_title(f"Top {len(t)} traços morfológicos (a contagem mistura classes)")
    finalizar(fig, caminho, mostrar)
