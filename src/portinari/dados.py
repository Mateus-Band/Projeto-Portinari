"""Funções utilitárias para carregamento padronizado dos dados do Projeto Portinari."""

import json
from pathlib import Path
from typing import Any, List, Optional
import pandas as pd

from portinari import caminhos

_MENSAGEM_ERRO_DRIVE = (
    "\n\n[ERRO: DADOS NÃO ENCONTRADOS LOCALMENTE]\n"
    "Os dados e imagens do Projeto Portinari são privados e residem no Google Drive institucional.\n"
    "Para utilizá-los:\n"
    "1. Solicite acesso ao Google Drive do projeto via e-mail institucional.\n"
    "2. Baixe os dados ou sincronize via rclone para a pasta local 'data/' (já coberta no .gitignore).\n"
    "3. Consulte o README.md para instruções detalhadas."
)


def _verificar_existencia(caminho: Path) -> None:
    if not caminho.exists():
        raise FileNotFoundError(f"Arquivo não encontrado: {caminho}{_MENSAGEM_ERRO_DRIVE}")


def carregar_obras(caminho: Optional[Path] = None) -> pd.DataFrame:
    """Carrega os metadados tabulares das obras (data/processados/obras.csv)."""
    p = caminho or caminhos.OBRAS_CSV
    _verificar_existencia(p)
    return pd.read_csv(p, encoding="utf-8")


def carregar_obras_limpo(caminho: Optional[Path] = None) -> List[dict[str, Any]]:
    """Carrega os metadados estruturados das obras (data/processados/obras_limpo.json)."""
    p = caminho or caminhos.OBRAS_LIMPO_JSON
    _verificar_existencia(p)
    with open(p, "r", encoding="utf-8") as f:
        return json.load(f)


def carregar_obras_cores(caminho: Optional[Path] = None) -> pd.DataFrame:
    """Carrega os metadados das obras enriquecidos com paletas de cores."""
    p = caminho or caminhos.OBRAS_CORES_CSV
    _verificar_existencia(p)
    return pd.read_csv(p, encoding="utf-8")


def carregar_poemas(caminho: Optional[Path] = None) -> pd.DataFrame:
    """Carrega a base principal de poemas (data/poemas/poemas.csv)."""
    p = caminho or caminhos.POEMAS_CSV
    _verificar_existencia(p)
    return pd.read_csv(p, encoding="utf-8")


def carregar_correspondencias(caminho: Optional[Path] = None) -> pd.DataFrame:
    """Carrega os dados tabulares das correspondências (cartas, telegramas)."""
    p = caminho or caminhos.CORRESPONDENCIAS_CSV
    _verificar_existencia(p)
    return pd.read_csv(p, encoding="utf-8")
