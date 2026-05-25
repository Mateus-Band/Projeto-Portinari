import pandas as pd

# Caminho do arquivo
input_path = "Data/obras.csv"

# Ler CSV
df = pd.read_csv(input_path)

# Filtrar linhas onde "temas" é nulo ou vazio
filtro = df["temas"].isna() | (df["temas"].str.strip() == "")

obras_sem_tema = df[filtro]

# Printar IDs
print("IDs das obras com 'temas' nulo ou vazio:")
for id_obra in obras_sem_tema["id"]:
    print(id_obra)