"""Preparação dos poemas: separação em versos, normalização de maiúsculas e tokens comuns.

O separador '/' serve só como informação do pipeline: é removido antes de o texto chegar ao
modelo. Os versos são unidos por UM ESPAÇO (nunca por quebra de linha, que vira token em alguns
modelos e interrompe frases que atravessam vários versos, como em "Não / Há rio").
"""
from __future__ import annotations

import re
from bisect import bisect_right
from dataclasses import dataclass

import pandas as pd

# Token comum: sequência de letras, com hífen ou apóstrofo internos ("bem-estar", "d'água").
# Pontuação e números em dígitos ficam de fora.
TOKEN_RE = re.compile(r"[^\W\d_]+(?:[-'’][^\W\d_]+)*", re.UNICODE)
SENT_RE = re.compile(r"[.!?…]+")
FIM_DE_FRASE = re.compile(r"[.!?…][\"'”’»)\]]*$")     # texto que termina uma frase (aceita aspas/parêntese depois)
PONTUACAO = ".,;:!?…\"'“”‘’()[]«»—–"
MAIUSCULA_INICIAL = re.compile(r"^(\W*)([A-ZÁÀÂÃÉÊÍÓÔÕÚÇ])")


@dataclass(frozen=True)
class TokenComum:
    idx: int          # posição do token no poema (0, 1, 2, ...)
    verso_idx: int    # verso em que o token está
    ini: int          # posição do 1º caractere no texto do poema (versos unidos por um espaço)
    fim: int          # posição seguinte ao último caractere
    texto: str        # a palavra como foi entregue ao modelo (já normalizada, se for o caso)


@dataclass
class PoemaPreparado:
    poema_id: str
    versos: list[str]          # versos, já normalizados (se NORMALIZAR_MAIUSCULAS)
    texto: str                 # versos unidos por um espaço
    inicios: list[int]         # posição de cada verso dentro de `texto`
    tokens: list[TokenComum]


def separar_versos(texto: str) -> list[str]:
    """Divide o poema nos versos (separados por '/'), sem espaços sobrando nem versos vazios."""
    return [re.sub(r"\s+", " ", v).strip() for v in texto.split("/") if v.strip()]


def texto_do_poema(texto_bruto: str) -> str:
    """Texto do poema sem as barras: versos unidos por um espaço, sem normalizar maiúsculas."""
    return " ".join(separar_versos(texto_bruto))


def capitalizadas_no_meio(textos) -> frozenset[str]:
    """Palavras que aparecem com maiúscula no meio de um verso, fora de início de frase.

    Em português, maiúscula no meio da frase indica nome próprio (Emily, Brodowski).
    O conjunto serve para não rebaixar essas palavras quando elas abrem um verso.
    """
    achadas = set()
    for texto in textos:
        for verso in separar_versos(texto):
            palavras = verso.split(" ")
            for anterior, palavra in zip(palavras, palavras[1:]):
                if palavra[:1].isupper() and not FIM_DE_FRASE.search(anterior):
                    achadas.add(palavra.strip(PONTUACAO))
    return frozenset(achadas)


def normalizar_versos(versos: list[str], preservar: frozenset[str] = frozenset()) -> list[str]:
    """Rebaixa a maiúscula inicial dos versos que continuam a frase do verso anterior.

    1. O primeiro verso e os versos que começam uma frase ficam como estão. Um verso começa uma
       frase quando o verso anterior termina em '.', '!', '?' ou '…' (aceitando aspas, parênteses
       ou colchetes depois).
    2. Se o verso anterior NÃO termina assim, a maiúscula inicial do verso é rebaixada.
    3. Exceção: palavras do conjunto `preservar` (nomes próprios, ver capitalizadas_no_meio).

    Maiúsculas no meio do verso nunca são alteradas.
    """
    saida = []
    for i, verso in enumerate(versos):
        continua_frase = i > 0 and not FIM_DE_FRASE.search(versos[i - 1])
        primeira_palavra = verso.split(" ", 1)[0].strip(PONTUACAO)
        if continua_frase and primeira_palavra not in preservar:
            verso = MAIUSCULA_INICIAL.sub(lambda m: m.group(1) + m.group(2).lower(), verso)
        saida.append(verso)
    return saida


def extrair_tokens(texto: str, inicios: list[int]) -> list[TokenComum]:
    """Tokens comuns do texto do poema, com a posição e o verso de cada um."""
    tokens = []
    for idx, m in enumerate(TOKEN_RE.finditer(texto)):
        verso_idx = max(bisect_right(inicios, m.start()) - 1, 0)
        tokens.append(TokenComum(idx, verso_idx, m.start(), m.end(), m.group()))
    return tokens


def preparar_poema(poema_id: str, texto_bruto: str, normalizar: bool = True,
                   preservar: frozenset[str] = frozenset()) -> PoemaPreparado:
    versos = separar_versos(texto_bruto)
    if normalizar:
        versos = normalizar_versos(versos, preservar)
    inicios, pos = [], 0
    for v in versos:
        inicios.append(pos)
        pos += len(v) + 1                 # +1: o espaço que separa os versos
    texto = " ".join(versos)
    return PoemaPreparado(poema_id, versos, texto, inicios, extrair_tokens(texto, inicios))


def preparar_corpus(corpus: pd.DataFrame, normalizar: bool = True) -> list[PoemaPreparado]:
    """Prepara todos os poemas. O conjunto de nomes protegidos é calculado sobre o corpus inteiro."""
    preservar = capitalizadas_no_meio(corpus["texto"]) if normalizar else frozenset()
    return [
        preparar_poema(pid, texto, normalizar, preservar)
        for pid, texto in zip(corpus["poema_id"], corpus["texto"])
    ]
