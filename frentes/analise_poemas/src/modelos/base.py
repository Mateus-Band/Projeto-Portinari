"""Interface comum dos modelos de PLN. Este módulo não importa nenhuma biblioteca de PLN."""
from __future__ import annotations

from dataclasses import dataclass

# O que cada modelo fornece. Quando algo não é fornecido, a coluna correspondente fica vazia nos CSVs
# e os notebooks 02 e 03 avisam e pulam o que depende dela.
CAPACIDADES = {
    "spacy":        {"upos": True, "morph": True,       "lema": True},
    "stanza":       {"upos": True, "morph": True,       "lema": True},
    "udpipe2":      {"upos": True, "morph": True,       "lema": True},
    "trankit":      {"upos": True, "morph": True,       "lema": True},
    "bertimbau_ft": {"upos": True, "morph": "opcional", "lema": False},
}

# As 17 classes gramaticais do Universal Dependencies (UPOS).
UPOS_VALIDOS = frozenset(
    "NOUN PROPN VERB AUX ADJ ADV PRON DET ADP CCONJ SCONJ NUM INTJ PART X PUNCT SYM".split()
)


@dataclass(frozen=True)
class PalavraModelo:
    """Uma palavra (ou componente de contração) devolvida pelo modelo.

    `ini` e `fim` são posições de caracteres no texto que foi entregue ao modelo (fim exclusivo).
    Componentes de uma mesma contração ("do" = "de" + "o") compartilham o mesmo `ini`/`fim`.
    """
    ini: int
    fim: int
    texto: str
    lema: str = ""
    upos: str = ""
    morph: str = ""


class Modelo:
    """Interface: todo modelo implementa `analisar`, `versao` e `acesso`."""

    nome: str = ""
    fornece_lema: bool = True
    fornece_morph: bool = True

    def versao(self) -> str:
        raise NotImplementedError

    def acesso(self) -> str:
        """'local' ou 'servico online'."""
        return "local"

    def analisar(self, textos: list[str]) -> list[list[PalavraModelo]]:
        """Analisa cada texto de forma independente e devolve, para cada um, a lista de palavras
        (incluindo pontuação, que o alinhamento descarta) com posições relativas ao texto."""
        raise NotImplementedError
