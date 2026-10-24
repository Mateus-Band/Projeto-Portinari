"""UDPipe 2: modelo UD do português, via serviço REST do LINDAT (ou instalação local, não implementada).

ATENÇÃO: este adaptador foi escrito a partir da documentação do serviço e NÃO foi executado contra o
servidor real (o ambiente de desenvolvimento não o alcança). O analisador de CoNLL-U é testado com
saídas simuladas. Confirme o nome do modelo com `listar_modelos()` antes do primeiro uso.

Privacidade e reprodutibilidade: o serviço online ENVIA OS POEMAS A UM SERVIDOR EXTERNO e o modelo
pode mudar com o tempo. Por isso o nome do modelo devolvido pelo servidor e a data entram no registro.
Os modelos são livres para uso não comercial.
"""
from __future__ import annotations

import re
from typing import Iterable

from .base import Modelo, PalavraModelo

URL_API = "https://lindat.mff.cuni.cz/services/udpipe/api"
MODELO_PADRAO = "portuguese-bosque-ud-2.12-230717"      # confirmar com listar_modelos()
FAIXA = re.compile(r"TokenRange=(\d+):(\d+)")


def listar_modelos(url: str = URL_API, timeout: int = 60) -> dict:
    """Consulta o serviço e devolve o JSON com os modelos disponíveis."""
    import requests

    resposta = requests.get(f"{url}/models", timeout=timeout)
    resposta.raise_for_status()
    return resposta.json()


def parsear_conllu(conllu: str, texto: str) -> list[PalavraModelo]:
    """Converte a saída CoNLL-U do UDPipe em PalavraModelo, com posições no `texto` enviado.

    As posições vêm de `TokenRange=ini:fim` (campo MISC, opção `tokenizer=ranges`). Sem esse campo,
    cada forma é procurada em sequência no texto. Em tokens multipalavra (linhas com id `3-4`),
    todas as palavras componentes recebem a posição do token.
    """
    palavras: list[PalavraModelo] = []
    busca = 0

    def posicao(forma: str, misc: str) -> tuple[int, int]:
        nonlocal busca
        m = FAIXA.search(misc)
        if m:
            ini, fim = int(m.group(1)), int(m.group(2))
        else:
            ini = texto.find(forma, busca)
            if ini < 0:
                ini = busca
            fim = ini + len(forma)
        busca = fim
        return ini, fim

    faixa_mwt: tuple[tuple[int, int], int] | None = None     # (posição, id da última componente)
    for linha in conllu.splitlines():
        if not linha or linha.startswith("#"):
            continue
        campos = linha.split("\t")
        if len(campos) < 10:
            continue
        id_, forma, lema, upos, _xpos, feats, _head, _deprel, _deps, misc = campos[:10]
        if "." in id_:                                     # nó vazio (elipse): sem posição no texto
            continue
        if "-" in id_:                                     # token multipalavra: "3-4  do"
            ultimo = int(id_.split("-")[1])
            faixa_mwt = (posicao(forma, misc), ultimo)
            continue
        n = int(id_)
        if faixa_mwt is not None and n <= faixa_mwt[1]:
            ini, fim = faixa_mwt[0]
        else:
            faixa_mwt = None
            ini, fim = posicao(forma, misc)
        palavras.append(PalavraModelo(
            ini, fim, forma, "" if lema == "_" else lema, upos, "" if feats == "_" else feats
        ))
    return palavras


class ModeloUDPipe2(Modelo):
    nome = "udpipe2"

    def __init__(self, acesso: str = "rest", modelo: str = MODELO_PADRAO, url: str = URL_API,
                 lote: str = "bloco", max_caracteres: int = 50_000, timeout: int = 300):
        """acesso: 'rest' (serviço do LINDAT) ou 'local' (não implementado).
        lote: 'bloco' envia vários textos por requisição, separados por linha em branco (poupa o
              servidor nos contextos verso/token); 'individual' envia um texto por requisição."""
        if acesso == "local":
            raise NotImplementedError(
                "UDPipe 2 local exige o repositório udpipe/udpipe2 e GPU e não foi implementado. "
                "Use acesso='rest' ou outro modelo."
            )
        if acesso != "rest":
            raise ValueError("acesso deve ser 'rest' ou 'local'.")
        if lote not in ("bloco", "individual"):
            raise ValueError("lote deve ser 'bloco' ou 'individual'.")
        try:
            import requests
        except ImportError as erro:
            raise ImportError("pip install requests") from erro
        self._requests = requests
        self.modelo, self.url, self.lote = modelo, url, lote
        self.max_caracteres, self.timeout = max_caracteres, timeout
        self._modelo_servidor: str | None = None

    def versao(self) -> str:
        return f"udpipe2 rest; modelo={self._modelo_servidor or self.modelo}"

    def acesso(self) -> str:
        return "servico online (LINDAT)"

    def _processar(self, texto: str) -> str:
        resposta = self._requests.post(
            f"{self.url}/process",
            data={"model": self.modelo, "tokenizer": "ranges", "tagger": "", "data": texto},
            timeout=self.timeout,
        )
        resposta.raise_for_status()
        corpo = resposta.json()
        self._modelo_servidor = corpo.get("model", self._modelo_servidor)
        return corpo["result"]

    def _blocos(self, textos: list[str]) -> Iterable[list[int]]:
        """Agrupa índices de textos em blocos de até `max_caracteres` caracteres."""
        bloco, tamanho = [], 0
        for i, t in enumerate(textos):
            if bloco and (self.lote == "individual" or tamanho + len(t) + 2 > self.max_caracteres):
                yield bloco
                bloco, tamanho = [], 0
            bloco.append(i)
            tamanho += len(t) + 2
        if bloco:
            yield bloco

    def analisar(self, textos: list[str]) -> list[list[PalavraModelo]]:
        saidas: list[list[PalavraModelo]] = [[] for _ in textos]
        indices = [i for i, t in enumerate(textos) if t.strip()]
        vazios_removidos = [textos[i] for i in indices]
        for bloco in self._blocos(vazios_removidos):
            partes = [vazios_removidos[k] for k in bloco]
            enviado = "\n\n".join(partes)                  # linha em branco = fronteira de frase
            inicios, pos = [], 0
            for p in partes:
                inicios.append(pos)
                pos += len(p) + 2
            palavras = parsear_conllu(self._processar(enviado), enviado)
            for p in palavras:
                # a qual texto do bloco a palavra pertence?
                k = max(j for j, ini in enumerate(inicios) if ini <= p.ini)
                saidas[indices[bloco[k]]].append(
                    PalavraModelo(p.ini - inicios[k], p.fim - inicios[k], p.texto, p.lema, p.upos, p.morph)
                )
        return saidas
