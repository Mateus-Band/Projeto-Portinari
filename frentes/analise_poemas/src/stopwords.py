"""Lista única de stopwords do projeto (palavras funcionais do português).

A lista é a união da lista própria abaixo com a do spaCy, CONGELADA em src/stopwords_pt.txt, para
que os notebooks 02 e 03 não dependam do spaCy nem da sua versão. Para regenerar o arquivo
(só se a lista própria mudar; exige spaCy):  python -m src.stopwords

As stopwords são aplicadas à unidade contada (lema ou forma), nunca antes do modelo.
"""
from __future__ import annotations

from functools import lru_cache
from pathlib import Path

ARQUIVO = Path(__file__).with_name("stopwords_pt.txt")

# Stopwords do português (palavras funcionais).
STOPWORDS_PROPRIAS = frozenset("""
a o as os um uma uns umas de do da dos das dum duma duns dumas em no na nos nas num numa nuns numas
por pelo pela pelos pelas per para pra com sem sob sobre entre até ante após contra desde perante
e ou mas porém contudo todavia senão nem que se como quando porque pois porquanto embora conquanto
eu tu ele ela nós vós eles elas me te lhe lhes nos vos mim ti si comigo contigo consigo
meu minha meus minhas teu tua teus tuas seu sua seus suas nosso nossa nossos nossas
este esta estes estas esse essa esses essas aquele aquela aqueles aquelas isto isso aquilo
ao aos à às ali aqui aí lá cá onde
é são era eram foi foram ser sendo sido está estão estava estavam estar
há havia haver tem têm tinha tinham ter
já mais muito muita muitos muitas
""".split())


@lru_cache(maxsize=1)
def carregar_stopwords() -> frozenset[str]:
    if not ARQUIVO.exists():
        raise FileNotFoundError(f"{ARQUIVO} não encontrado. Gere-o com: python -m src.stopwords")
    linhas = ARQUIVO.read_text(encoding="utf-8").splitlines()
    return frozenset(l.strip() for l in linhas if l.strip() and not l.startswith("#"))


def gerar_arquivo() -> Path:
    """Regrava src/stopwords_pt.txt (lista própria ∪ spaCy). Utilitário de desenvolvimento."""
    from spacy.lang.pt.stop_words import STOP_WORDS      # import local: só aqui o spaCy é necessário

    todas = sorted(set(STOPWORDS_PROPRIAS) | {w.lower() for w in STOP_WORDS})
    cabecalho = ["# Stopwords do português: lista própria (src/stopwords.py) unida à do spaCy.",
                 "# Arquivo gerado; edite STOPWORDS_PROPRIAS e rode: python -m src.stopwords"]
    ARQUIVO.write_text("\n".join(cabecalho + todas) + "\n", encoding="utf-8")
    carregar_stopwords.cache_clear()
    return ARQUIVO


if __name__ == "__main__":
    caminho = gerar_arquivo()
    print(f"{len(carregar_stopwords())} stopwords gravadas em {caminho}")
