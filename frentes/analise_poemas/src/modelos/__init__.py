"""Modelos de PLN, todos com a mesma interface (ver base.Modelo).

Os módulos de cada modelo só são importados quando o modelo é pedido, para que o pacote possa
ser importado sem ter spaCy, Stanza, Trankit etc. instalados.
"""
from .base import CAPACIDADES, UPOS_VALIDOS, Modelo, PalavraModelo

NOMES_MODELOS = tuple(CAPACIDADES)


def criar_modelo(nome: str, **configuracao) -> Modelo:
    """Cria o modelo `nome` (spacy, stanza, udpipe2, trankit, bertimbau_ft)."""
    if nome == "spacy":
        from .spacy_ import ModeloSpacy
        return ModeloSpacy(**configuracao)
    if nome == "stanza":
        from .stanza_ import ModeloStanza
        return ModeloStanza(**configuracao)
    if nome == "udpipe2":
        from .udpipe2 import ModeloUDPipe2
        return ModeloUDPipe2(**configuracao)
    if nome == "trankit":
        from .trankit_ import ModeloTrankit
        return ModeloTrankit(**configuracao)
    if nome == "bertimbau_ft":
        from .bertimbau_ft import ModeloBertimbauFT
        return ModeloBertimbauFT(**configuracao)
    raise ValueError(f"Modelo desconhecido: '{nome}'. Opções: {', '.join(NOMES_MODELOS)}")
