"""spaCy: pt_core_news_lg (ou md/sm), com NER e parser desligados."""
from __future__ import annotations

from .base import Modelo, PalavraModelo


class ModeloSpacy(Modelo):
    nome = "spacy"

    def __init__(self, pacote: str = "pt_core_news_lg", tamanho_lote: int = 32):
        try:
            import spacy
        except ImportError as erro:
            raise ImportError("spaCy não instalado: pip install spacy") from erro
        try:
            self.nlp = spacy.load(pacote, disable=["ner", "parser"])
        except OSError as erro:
            raise RuntimeError(
                f"Pacote '{pacote}' não encontrado. Instale com: python -m spacy download {pacote} "
                "(alternativas: pt_core_news_md, pt_core_news_sm). Não há troca automática de pacote: "
                "o pacote usado precisa constar no registro."
            ) from erro
        self.pacote = pacote
        self.tamanho_lote = tamanho_lote
        self._spacy = spacy

    def versao(self) -> str:
        return f"{self.pacote} {self.nlp.meta['version']}; spacy {self._spacy.__version__}"

    def analisar(self, textos: list[str]) -> list[list[PalavraModelo]]:
        saidas = []
        for doc in self.nlp.pipe(textos, batch_size=self.tamanho_lote):
            saidas.append([
                PalavraModelo(t.idx, t.idx + len(t.text), t.text, t.lemma_, t.pos_, str(t.morph))
                for t in doc
                if not t.is_space
            ])
        return saidas
