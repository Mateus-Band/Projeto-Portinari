# contar quantas palavras tem no total na coluna "estrofes" de poemas.csv

from pathlib import Path

# Caminho do arquivo poemas.csv (relativo ao diretório do script)
caminho_csv = Path(__file__).resolve().parent / "poemas.csv"

try:
    import pandas as pd

    df = pd.read_csv(caminho_csv)
    estrofes = df["estrofes"].dropna().astype(str).tolist()
except ImportError:
    import csv

    with open(caminho_csv, mode="r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        estrofes = [row["estrofes"] for row in reader if row.get("estrofes")]

# Contagem total de palavras separadas por espaços em branco
total_palavras = sum(len(estrofe.split()) for estrofe in estrofes)

# Contagem desconsiderando símbolos/pontuações isoladas (ex: "—", "–")
total_palavras_sem_simbolos = sum(
    1
    for estrofe in estrofes
    for palavra in estrofe.split()
    if any(caractere.isalnum() for caractere in palavra)
)

print(f"Total de poemas analisados: {len(estrofes)}")
print(f"Total de palavras na coluna 'estrofes': {total_palavras}")
print(f"Total de palavras (desconsiderando pontuações isoladas): {total_palavras_sem_simbolos}")
