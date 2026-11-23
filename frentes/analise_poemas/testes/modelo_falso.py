"""Modelo de mentira para testar o pipeline sem baixar nenhum modelo de PLN.

Tokeniza por regex (palavras e pontuação) e SEPARA contrações como Stanza/UDPipe/Trankit:
as componentes de "do" herdam a posição do token inteiro. Com `separar_hifen=True`, imita um
tokenizador que parte "bem-estar" em "bem", "-" e "estar".
"""
import re

from src.modelos.base import Modelo, PalavraModelo

CONTRACOES = {"do": ("de", "o"), "no": ("em", "o"), "dela": ("de", "ela"), "pela": ("por", "a"), "na": ("em", "a")}
TOKEN = re.compile(r"\w+(?:['’-]\w+)*|[^\w\s]", re.UNICODE)


class ModeloFalso(Modelo):
    nome = "falso"

    def __init__(self, fornece_lema=True, fornece_morph=True, separar_hifen=False):
        self.fornece_lema, self.fornece_morph, self.separar_hifen = fornece_lema, fornece_morph, separar_hifen
        self.chamadas = []

    def versao(self):
        return "modelo falso 1.0"

    def analisar(self, textos):
        self.chamadas.append(list(textos))
        saidas = []
        for texto in textos:
            palavras = []
            for m in TOKEN.finditer(texto):
                forma, ini, fim = m.group(), m.start(), m.end()
                baixa = forma.lower()
                if not re.match(r"\w", forma):
                    palavras.append(PalavraModelo(ini, fim, forma, forma, "PUNCT", ""))
                elif baixa in CONTRACOES:
                    a, b = CONTRACOES[baixa]
                    palavras.append(PalavraModelo(ini, fim, a, a, "ADP", ""))
                    palavras.append(PalavraModelo(ini, fim, b, b, "DET", "Definite=Def"))
                elif self.separar_hifen and "-" in forma:
                    pos = ini
                    for parte in re.split(r"(-)", forma):
                        if parte:
                            upos = "PUNCT" if parte == "-" else "NOUN"
                            palavras.append(PalavraModelo(pos, pos + len(parte), parte, parte.lower(), upos, ""))
                            pos += len(parte)
                else:
                    upos = "PROPN" if forma[0].isupper() and ini > 0 else "NOUN"
                    palavras.append(PalavraModelo(ini, fim, forma, baixa, upos, "Gender=Masc|Number=Sing"))
            saidas.append(palavras)
        return saidas
