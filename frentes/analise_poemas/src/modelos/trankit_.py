"""Trankit: pipeline em português (baseado em XLM-RoBERTa).

ATENÇÃO: este adaptador foi escrito a partir da documentação do Trankit e NÃO foi executado (o ambiente
de desenvolvimento não baixa os modelos). Antes de investir nele, confirme que o Trankit instala e roda
no seu ambiente (ele costuma exigir versões específicas de torch/transformers) e que ainda é mantido.
"""
from __future__ import annotations

from .base import Modelo, PalavraModelo


class ModeloTrankit(Modelo):
    nome = "trankit"

    def __init__(self, idioma: str = "portuguese", usar_gpu: bool = False, cache_dir: str | None = None):
        try:
            import trankit
            from trankit import Pipeline
        except ImportError as erro:
            raise ImportError(
                "Trankit não instalado (pip install trankit). Se a instalação falhar, veja o README: "
                "o Trankit é o modelo de maior risco de compatibilidade."
            ) from erro
        self._trankit = trankit
        self.idioma = idioma
        kwargs = {"gpu": usar_gpu}
        if cache_dir:
            kwargs["cache_dir"] = cache_dir
        try:
            self.pipeline = Pipeline(idioma, **kwargs)
        except Exception as erro:
            raise RuntimeError(f"Não foi possível criar o pipeline do Trankit ('{idioma}'): {erro}") from erro

    def versao(self) -> str:
        return f"trankit {getattr(self._trankit, '__version__', 'desconhecida')}; lang={self.idioma}"

    @staticmethod
    def _limites(token: dict, texto: str) -> tuple[int, int]:
        """Posição do token no documento. `dspan` = (início, fim); confere o fim exclusivo/inclusivo."""
        ini, fim = token["dspan"]
        forma = token.get("text", "")
        if texto[ini:fim] != forma and texto[ini:fim + 1] == forma:
            fim += 1
        return ini, fim

    def analisar(self, textos: list[str]) -> list[list[PalavraModelo]]:
        saidas = []
        for texto in textos:
            if not texto.strip():
                saidas.append([])
                continue
            resultado = self.pipeline(texto)
            palavras = []
            for sentenca in resultado.get("sentences", []):
                for token in sentenca.get("tokens", []):
                    ini, fim = self._limites(token, texto)
                    # Contração: o token traz as palavras componentes em 'expanded'.
                    for w in token.get("expanded", [token]):
                        palavras.append(PalavraModelo(
                            ini, fim, w.get("text", ""), w.get("lemma", "") or "",
                            w.get("upos", "") or "", w.get("feats", "") or "",
                        ))
            saidas.append(palavras)
        return saidas
