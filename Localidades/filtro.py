import pandas as pd

# Caminhos
input_path = "Data/obras.csv"
output_path = "Localidades/Localidades_Data/obras_filtrado.csv"

# Ler CSV
df = pd.read_csv(input_path)

# 🔹 Remover títulos com "retrato" (case insensitive)
# df = df[~df["titulo"].str.contains("retrato", case=False, na=False)]

# 🔹 Remover obras com tema "Família do artista"
# df = df[~df["temas"].str.contains("Família do artista", case=False, na=False)]

# 🔹 Temas relevantes
temas_relevantes = {
    "Rio de Janeiro", "Brodowski", "Cidade", "Favela", "Morro",
    "Paisagem", "Campo", "Floresta", "Marinha", "Deserto",
    "Cultura de café", "Cultura de arroz", "Cultura de cana", "Cultura de fumo",
    "Cultura de milho", "Cultura de cacau", "Cultura de algodão",
    "Cultura de borracha", "Cultura de erva-mate",
    "Lavoura", "Garimpagem",
    "Trabalhadores", "Cenas de trabalho", "Garimpeiro", "Lavrador",
    "Operário",
    "Festas populares", "Carnaval", "Circo",
    "Guerra", "Cenas Históricas", "Descobrimento do Brasil",
}

# 🔹 Função para filtrar e remover duplicatas
def filtrar_temas(temas_str):
    if pd.isna(temas_str):
        return []
    
    lista = [t.strip() for t in temas_str.split(",") if t.strip() != ""]
    
    # 🔹 Remover duplicatas mantendo ordem
    lista_unica = list(dict.fromkeys(lista))
    
    # 🔹 Filtrar relevantes
    relevantes = [t for t in lista_unica if t in temas_relevantes]
    
    return relevantes

# Aplicar filtro
df["temas_filtrados"] = df["temas"].apply(filtrar_temas)

# 🔹 Remover obras sem temas relevantes
df = df[df["temas_filtrados"].apply(len) > 0]

# 🔹 Converter lista para string
df["temas_filtrados"] = df["temas_filtrados"].apply(lambda x: ", ".join(x))

# 🔹 Colunas finais
colunas_desejadas = [
    "id",
    "imagemUrl",
    "titulo",
    "tipoObra",
    "temas_filtrados"
]

df_final = df[colunas_desejadas]

# Salvar CSV
df_final.to_csv(output_path, index=False, encoding="utf-8")

print(f"Novo arquivo criado em: {output_path}")