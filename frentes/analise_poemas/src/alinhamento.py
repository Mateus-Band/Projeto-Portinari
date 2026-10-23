"""Alinhamento das palavras de cada modelo aos tokens comuns, pela posição dos caracteres.

Cada modelo tokeniza por conta própria. Para que todos acabem com os mesmos tokens por poema, as
palavras do modelo são alinhadas aos tokens comuns (preprocessamento.TokenComum):

* uma palavra cobrindo exatamente o token: caso normal;
* várias palavras cobrindo exatamente o token (contração separada, como "do" = "de" + "o", ou um
  clítico, como "dá-se" = "dá" + "se"): são REAGRUPADAS, com lema = componentes unidos por espaço
  ("de o") e classe e traços = os da primeira componente;
* limites que não coincidem (palavra do modelo maior que o token, ou token sem correspondência):
  DIVERGÊNCIA de alinhamento: usa-se a palavra de maior sobreposição e a divergência é registrada.
"""
from __future__ import annotations

from dataclasses import dataclass, field

from .modelos.base import PalavraModelo

UPOS_PONTUACAO = frozenset({"PUNCT", "SYM"})
LACUNAS_PERMITIDAS = frozenset("-'’")      # caracteres do token que o modelo pode deixar fora (hífen, apóstrofo)


@dataclass
class ResultadoAlinhamento:
    linhas: list[dict]                                        # um por token comum: lema, upos, morph
    n_reagrupados: int = 0
    divergencias: list[dict] = field(default_factory=list)    # token_idx, token, motivo, palavras_modelo

    @property
    def n_divergencias(self) -> int:
        return len(self.divergencias)


def _cobre(token, palavras: list[PalavraModelo]) -> bool:
    """As palavras cobrem todos os caracteres do token (exceto hífen/apóstrofo)?"""
    cobertos = [False] * (token.fim - token.ini)
    for p in palavras:
        for k in range(max(p.ini, token.ini), min(p.fim, token.fim)):
            cobertos[k - token.ini] = True
    return all(c or token.texto[i] in LACUNAS_PERMITIDAS for i, c in enumerate(cobertos))


def _linha(p: PalavraModelo | None, lema: str | None = None) -> dict: #padronizar as linhas
    if p is None:
        return {"lema": "", "upos": "", "morph": ""}
    return {"lema": p.lema if lema is None else lema, "upos": p.upos, "morph": p.morph}


def alinhar(tokens, palavras: list[PalavraModelo]) -> ResultadoAlinhamento:
    """Alinha `palavras` (posições no texto do poema) aos `tokens` comuns do poema."""
    palavras = sorted((p for p in palavras if p.fim > p.ini), key=lambda p: (p.ini, p.fim))
    resultado = ResultadoAlinhamento(linhas=[])
    inicio = 0
    for token in tokens:
        while inicio < len(palavras) and palavras[inicio].fim <= token.ini:
            inicio += 1
        cobertura, j = [], inicio
        while j < len(palavras) and palavras[j].ini < token.fim:
            if palavras[j].fim > token.ini:
                cobertura.append(palavras[j])
            j += 1

        if not cobertura:
            resultado.linhas.append(_linha(None))
            resultado.divergencias.append(
                {"token_idx": token.idx, "token": token.texto, "motivo": "sem correspondência", "palavras_modelo": ""}
            )
            continue

        dentro = all(p.ini >= token.ini and p.fim <= token.fim for p in cobertura)
        if dentro and _cobre(token, cobertura):
            componentes = [p for p in cobertura if p.upos not in UPOS_PONTUACAO] or cobertura
            if len(cobertura) > 1:
                resultado.n_reagrupados += 1
            lema = " ".join(p.lema for p in componentes if p.lema)
            resultado.linhas.append(_linha(componentes[0], lema))
        else:
            melhor = max(cobertura, key=lambda p: min(p.fim, token.fim) - max(p.ini, token.ini))
            resultado.linhas.append(_linha(melhor))
            resultado.divergencias.append({
                "token_idx": token.idx, "token": token.texto, "motivo": "limites não coincidem",
                "palavras_modelo": " | ".join(f"{p.texto}[{p.ini}:{p.fim}]" for p in cobertura),
            })
    return resultado
