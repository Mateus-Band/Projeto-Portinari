"""Pacote central compartilhado do Projeto Portinari."""

from portinari import caminhos
from portinari.dados import (
    carregar_correspondencias,
    carregar_obras,
    carregar_obras_cores,
    carregar_obras_limpo,
    carregar_poemas,
)

__all__ = [
    "caminhos",
    "carregar_obras",
    "carregar_obras_limpo",
    "carregar_obras_cores",
    "carregar_poemas",
    "carregar_correspondencias",
]
