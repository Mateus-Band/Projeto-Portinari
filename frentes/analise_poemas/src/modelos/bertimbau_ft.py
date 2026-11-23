"""bertimbau_ft (FASE 2): BERTimbau ajustado (fine-tuning) para classe gramatical.

O BERTimbau base (neuralmind/bert-base-portuguese-cased) é um codificador pré-treinado, não um
etiquetador: precisa de uma cabeça de classificação de tokens treinada em corpus UD do português
(Bosque ou GSD). Receberá as palavras já tokenizadas (token comum), e o rótulo de cada palavra virá
da primeira subpalavra. UPOS é obrigatório; traços, só se a cabeça for treinada para eles; sem lema.
"""
from __future__ import annotations

from .base import Modelo


class ModeloBertimbauFT(Modelo):
    nome = "bertimbau_ft"
    fornece_lema = False
    fornece_morph = False

    def __init__(self, *args, **kwargs):
        raise NotImplementedError(
            "MODELO = 'bertimbau_ft' ainda não existe (fase 2). O BERTimbau base não etiqueta sozinho: "
            "é preciso treinar a cabeça de classificação de classes gramaticais em um corpus UD do português "
            "(Bosque ou GSD) e avaliá-la na amostra anotada à mão (dados/anotado/). "
            "Enquanto isso, use MODELO = 'spacy', 'stanza', 'udpipe2' ou 'trankit'."
        )
