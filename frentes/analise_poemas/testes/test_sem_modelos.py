"""Critério: os módulos usados pelos notebooks 02 e 03 não importam nenhuma biblioteca de PLN."""
import subprocess
import sys
from pathlib import Path

RAIZ = Path(__file__).resolve().parents[1]
PROIBIDOS = ["spacy", "stanza", "trankit", "torch", "transformers", "ufal", "udpipe", "thinc"]


def test_modulos_dos_notebooks_2_e_3_nao_carregam_modelos():
    codigo = (
        "import sys\n"
        f"sys.path.insert(0, r'{RAIZ}')\n"
        "from src import carga, metricas, graficos, stopwords, preprocessamento\n"
        "stopwords.carregar_stopwords()\n"
        f"ruins = [m for m in sys.modules if m.split('.')[0] in {PROIBIDOS!r}]\n"
        "print('CARREGADOS:' + ','.join(sorted(set(r.split('.')[0] for r in ruins))))\n"
    )
    saida = subprocess.run([sys.executable, "-c", codigo], capture_output=True, text=True, env={"MPLBACKEND": "Agg", "PATH": ""})
    assert saida.returncode == 0, saida.stderr
    assert saida.stdout.strip() == "CARREGADOS:", saida.stdout
