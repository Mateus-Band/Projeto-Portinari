"""Stanza: pipeline em português com tokenização, expansão de contrações, classe, traços e lema.

ATENÇÃO: este adaptador foi escrito a partir da documentação do Stanza e NÃO foi executado contra
os modelos reais (o ambiente de desenvolvimento não alcança o Hugging Face, de onde o Stanza baixa
os modelos). A lógica de alinhamento e de reagrupamento de contrações é testada com saídas simuladas.
"""
from __future__ import annotations

from .base import Modelo, PalavraModelo


class ModeloStanza(Modelo):
    nome = "stanza"

    def __init__(self, idioma: str = "pt", pacote: str = "default", usar_gpu: bool = False,
                 baixar: bool = False):
        try:
            import stanza
        except ImportError as erro:
            raise ImportError("Stanza não instalado: pip install stanza") from erro
        self._stanza = stanza
        self.idioma, self.pacote = idioma, pacote
        try:
            if baixar:
                stanza.download(idioma, package=pacote, verbose=False)
            self.nlp = stanza.Pipeline(
                idioma, package=pacote, processors="tokenize,mwt,pos,lemma",
                use_gpu=usar_gpu, verbose=False,
            )
        except Exception as erro:
            raise RuntimeError(
                f"Não foi possível criar o pipeline do Stanza ('{idioma}', pacote '{pacote}'): {erro}\n"
                f"Se os modelos não foram baixados: import stanza; stanza.download('{idioma}')"
            ) from erro

    def versao(self) -> str:
        return f"stanza {self._stanza.__version__}; lang={self.idioma}; package={self.pacote}"

    @staticmethod
    def _palavras(doc) -> list[PalavraModelo]:
        palavras = []
        for sentenca in doc.sentences:
            for token in sentenca.tokens:
                # Um token pode ter várias palavras (contração/clítico); todas herdam a posição do token.
                for w in token.words:
                    palavras.append(PalavraModelo(
                        token.start_char, token.end_char, w.text, w.lemma or "", w.upos or "", w.feats or ""
                    ))
        return palavras

    def analisar(self, textos: list[str]) -> list[list[PalavraModelo]]:
        saidas: list[list[PalavraModelo]] = [[] for _ in textos]
        pos_nao_vazios = [i for i, t in enumerate(textos) if t.strip()]
        if not pos_nao_vazios:
            return saidas
        entrada = [textos[i] for i in pos_nao_vazios]
        try:
            docs = self.nlp.bulk_process(entrada)          # processa em lote (mais rápido)
        except (AttributeError, TypeError):
            docs = [self.nlp(t) for t in entrada]
        for i, doc in zip(pos_nao_vazios, docs):
            saidas[i] = self._palavras(doc)
        return saidas
