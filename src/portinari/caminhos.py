"""Caminhos e diretórios padronizados para o Projeto Portinari.

Permite que scripts e notebooks acessem arquivos e pastas usando caminhos relativos
à raiz do repositório, independente de onde sejam executados ou do sistema operacional.
"""

from pathlib import Path

# Raiz do projeto (3 níveis acima deste arquivo: src/portinari/caminhos.py -> raiz)
RAIZ = Path(__file__).resolve().parents[2]

# Diretórios principais
DADOS_DIR = RAIZ / "dados"
DADOS_BRUTOS_DIR = DADOS_DIR / "brutos"
DADOS_PROCESSADOS_DIR = DADOS_DIR / "processados"
DADOS_POEMAS_DIR = DADOS_DIR / "poemas"
DADOS_PRIVADOS_DIR = DADOS_DIR / "privados"

ASSETS_DIR = RAIZ / "assets"
FRENTES_DIR = RAIZ / "frentes"
ARQUIVO_DIR = RAIZ / "arquivo"
APRESENTACOES_DIR = RAIZ / "apresentacoes"

# Frentes ativas
COLETA_DIR = FRENTES_DIR / "coleta"
CORES_DIR = FRENTES_DIR / "cores"
POEMAS_DIR = FRENTES_DIR / "poemas"
DUPLICATAS_DIR = FRENTES_DIR / "duplicatas"
CORRESPONDENCIAS_DIR = FRENTES_DIR / "correspondencias"
OCR_DIR = FRENTES_DIR / "ocr"

# Arquivos canônicos de metadados das obras
OBRAS_BRUTO_JSON = DADOS_BRUTOS_DIR / "obras.json"
OBRAS_CSV = DADOS_PROCESSADOS_DIR / "obras.csv"
OBRAS_LIMPO_JSON = DADOS_PROCESSADOS_DIR / "obras_limpo.json"
OBRAS_CORES_CSV = DADOS_PROCESSADOS_DIR / "obras_cores.csv"
CORRESPONDENCIAS_CSV = DADOS_PROCESSADOS_DIR / "correspondencias.csv"

# Arquivos de poemas
POEMAS_CSV = DADOS_POEMAS_DIR / "poemas.csv"
POEMAS_COM_OBRAS_CSV = DADOS_POEMAS_DIR / "poemas_com_obras.csv"
POEMAS_PRINCIPAIS_CSV = DADOS_POEMAS_DIR / "poemas_principais.csv"
PORTUGUESE_POEMS_CSV = DADOS_POEMAS_DIR / "portuguese-poems.csv"
POEMAS_PALAVRAS_CSV = DADOS_POEMAS_DIR / "poemas_palavras.csv"
