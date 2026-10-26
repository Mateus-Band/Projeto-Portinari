import pandas as pd
import matplotlib.pyplot as plt

# 1. Carrega as coordenadas do arquivo gerado pelo R
df = pd.read_csv('/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/frentes/poemas/descritiva/grafo/coordenadas_finais.csv')

# 2. Configura o tamanho e a resolução da figura
plt.figure(figsize=(12, 8))

# 3. Desenha os pontos (vértices)
plt.scatter(df['X'], df['Y'], s=150, c='dodgerblue', alpha=0.7, edgecolors='black')

'''
# 4. Adiciona o texto (nomes das palavras) para cada ponto
for indice, linha in df.iterrows():
    plt.annotate(
        linha['Palavra'], 
        (linha['X'], linha['Y']),
        xytext=(8, 0), # Desloca o texto 8 pixels para a direita
        textcoords='offset points',
        fontsize=12,
        fontweight='bold',
        verticalalignment='center'
    )'''

plt.title('Layout Multi-Dimensional do Grafo (GRIP)', fontsize=16, pad=15)
plt.grid(True, linestyle='--', alpha=0.4)
plt.xlabel('Posição X')
plt.ylabel('Posição Y')

# 6. Ajusta as margens e exibe a janela interativa
plt.tight_layout()
plt.savefig("grafo_grip.png")