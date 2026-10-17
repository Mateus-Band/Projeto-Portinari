from pathlib import Path
from portinari import caminhos

def test_raiz_do_projeto():
    """Testa se a constante RAIZ aponta para um diretório válido que contém o pyproject.toml."""
    assert caminhos.RAIZ.exists(), "O diretório raiz do projeto não existe"
    assert (caminhos.RAIZ / "pyproject.toml").exists(), "O arquivo pyproject.toml não foi encontrado na raiz do projeto"

def test_estrutura_diretorios_basicos():
    """Testa a existência (ou configuração correta do caminho) dos diretórios base."""
    # Diretórios que sempre existem no git
    assert caminhos.ASSETS_DIR.exists()
    assert caminhos.FRENTES_DIR.exists()

def test_pastas_frentes_ativas():
    """Verifica se os caminhos das frentes ativas estão configurados."""
    assert caminhos.COLETA_DIR.name == "coleta"
    assert caminhos.CORES_DIR.name == "cores"
    assert caminhos.POEMAS_DIR.name == "poemas"

def test_estrutura_dados_fallback():
    """Garante que DADOS_DIR aponte para um Path válido mesmo se a pasta não existir localmente no CI."""
    assert isinstance(caminhos.DADOS_DIR, Path)
    # Por padrão, quando a pasta não existe (ex: no CI), cai no fallback para "data"
    if not caminhos.DADOS_DIR.exists():
        assert caminhos.DADOS_DIR.name == "data"
