import pandas as pd
from collections import Counter

# Caminhos
input_path = "Data/obras.csv"
output_path = "Localidades/Localidades_Data/frequencia_temas.csv"

# Ler CSV
df = pd.read_csv(input_path)

# Contador
contador_temas = Counter()

# Processar temas
for temas in df["temas"].dropna():
    lista_temas = [t.strip() for t in temas.split(",") if t.strip() != ""]
    
    # remover duplicatas dentro da mesma obra
    lista_unica = set(lista_temas)
    
    contador_temas.update(lista_unica)

# Converter para DataFrame
df_freq = pd.DataFrame(contador_temas.items(), columns=["tema", "frequencia"])

# Ordenar decrescente
df_freq = df_freq.sort_values(by="frequencia", ascending=False)

# Salvar CSV
df_freq.to_csv(output_path, index=False, encoding="utf-8")

print(f"CSV criado em: {output_path}")