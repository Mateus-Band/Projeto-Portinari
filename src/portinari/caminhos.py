"""Caminhos e diretórios padronizados para o Projeto Portinari.

Permite que scripts e notebooks acessem arquivos e pastas usando caminhos relativos
à raiz do repositório, independente de onde sejam executados ou do sistema operacional.

NOTA: Todas as bases de dados e imagens residem no Google Drive privado do projeto
e devem ser sincronizadas ou descompactadas na pasta local 'data/' (ignorada pelo git).
"""

from pathlib import Path

# Raiz do projeto (3 níveis acima deste arquivo: src/portinari/caminhos.py -> raiz)
RAIZ = Path(__file__).resolve().parents[2]

# Diretório local de dados (sincronizado do Google Drive e ignorado pelo git)
# Procura prioritariamente pela pasta 'data/', com fallback para legados 'dados/' ou 'Data/'
if (RAIZ / "data").exists():
    DADOS_DIR = RAIZ / "data"
elif (RAIZ / "dados").exists():
    DADOS_DIR = RAIZ / "dados"
elif (RAIZ / "Data").exists():
    DADOS_DIR = RAIZ / "Data"
else:
    DADOS_DIR = RAIZ / "data"

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

# Arquivos de metadados das obras
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

# Imagens e documentos locais das obras (não versionados no git)
if (DADOS_DIR / "imagens").exists():
    IMAGENS_DIR = DADOS_DIR / "imagens"
elif (RAIZ / "dados-brutos" / "Imagens-Obras").exists():
    IMAGENS_DIR = RAIZ / "dados-brutos" / "Imagens-Obras"
elif (RAIZ / "Data" / "imagens").exists():
    IMAGENS_DIR = RAIZ / "Data" / "imagens"
else:
    IMAGENS_DIR = DADOS_DIR / "imagens"

if (DADOS_DIR / "documentos").exists():
    DOCUMENTOS_DIR = DADOS_DIR / "documentos"
elif (RAIZ / "dados-brutos" / "Correspondencias").exists():
    DOCUMENTOS_DIR = RAIZ / "dados-brutos" / "Correspondencias"
elif (RAIZ / "Data").exists():
    DOCUMENTOS_DIR = RAIZ / "Data"
else:
    DOCUMENTOS_DIR = DADOS_DIR / "documentos"
