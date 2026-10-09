import pandas as pd

# Caminho do CSV
input_path = "Localidades/Localidades_Data/frequencia_temas.csv"

# Ler CSV
df = pd.read_csv(input_path)

# 🔹 Lista de temas relevantes (mesma usada antes)
temas_relevantes = {
    "Rio de Janeiro", "Brodowski", "Cidade", "Favela", "Morro",
    "Natureza", "Paisagem", "Campo", "Floresta", "Marinha", "Deserto",
    "Cultura de café", "Cultura de arroz", "Cultura de cana", "Cultura de fumo",
    "Cultura de milho", "Cultura de cacau", "Cultura de algodão",
    "Cultura de borracha", "Cultura de erva-mate",
    "Lavoura", "Garimpagem",
    "Trabalhadores", "Cenas de trabalho", "Garimpeiro", "Lavrador",
    "Operário", "Industrialização",
    "Festas populares", "Carnaval", "Circo",
    "Guerra", "Cenas Históricas", "Descobrimento do Brasil",
}

# 🔹 Selecionar temas irrelevantes
temas_irrelevantes = df[~df["tema"].isin(temas_relevantes)]

# Mostrar lista
print(temas_irrelevantes["tema"].tolist())