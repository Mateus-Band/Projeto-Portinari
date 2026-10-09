"""Funções utilitárias para carregamento padronizado dos dados do Projeto Portinari."""

import json
from pathlib import Path
from typing import Any, List, Optional
import pandas as pd

from portinari import caminhos


def carregar_obras(caminho: Optional[Path] = None) -> pd.DataFrame:
    """Carrega os metadados tabulares das obras (dados/processados/obras.csv)."""
    p = caminho or caminhos.OBRAS_CSV
    return pd.read_csv(p, encoding="utf-8")


def carregar_obras_limpo(caminho: Optional[Path] = None) -> List[dict[str, Any]]:
    """Carrega os metadados estruturados das obras (dados/processados/obras_limpo.json)."""
    p = caminho or caminhos.OBRAS_LIMPO_JSON
    with open(p, "r", encoding="utf-8") as f:
        return json.load(f)


def carregar_obras_cores(caminho: Optional[Path] = None) -> pd.DataFrame:
    """Carrega os metadados das obras enriquecidos com paletas de cores."""
    p = caminho or caminhos.OBRAS_CORES_CSV
    return pd.read_csv(p, encoding="utf-8")


def carregar_poemas(caminho: Optional[Path] = None) -> pd.DataFrame:
    """Carrega a base principal de poemas (dados/poemas/poemas.csv)."""
    p = caminho or caminhos.POEMAS_CSV
    return pd.read_csv(p, encoding="utf-8")


def carregar_correspondencias(caminho: Optional[Path] = None) -> pd.DataFrame:
    """Carrega os dados tabulares das correspondências (cartas, telegramas)."""
    p = caminho or caminhos.CORRESPONDENCIAS_CSV
    return pd.read_csv(p, encoding="utf-8")
