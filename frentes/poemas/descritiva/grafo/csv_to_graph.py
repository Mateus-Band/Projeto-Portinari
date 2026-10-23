import pandas as pd
import networkx as nx
import numpy as np


'''
Esse programa tem como objetivo transformar matrizes de coocorência em csv para leitura mais facil do grafo, tirando as matrizes eparssas

basta dar uma matriz de coocorrência ordenada e será retornado uma matriz limpa e perfeta para o networkx
'''

def to_graphLike(dataframe,networkx = False):
    palavras = dataframe.columns[:-1].tolist()

    matriz = dataframe.iloc[:,:-1].to_numpy()

    matriz_superior = np.triu(matriz, k=1)
    linhas, colunas = np.where(matriz_superior > 0)

    new_df = pd.DataFrame()

    palavras = np.array(palavras)

    new_df['no_A'] = palavras[colunas]
    new_df['no_B'] = palavras[linhas]
    new_df['frequencia'] = matriz_superior[linhas,colunas]
    new_df['distacia'] = 1/new_df['frequencia']

    if networkx == True:
        G = nx.from_pandas_edgelist( df=new_df, source='no_A',target='no_B', edge_attr='frequencia', create_using=nx.Graph())
        nx.write_graphml(G, "GraphLike.graphml")
    else:
        new_df.to_csv('GraphLike.csv')




if __name__ == '__main__':
    df = pd.read_csv('/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/Poemas/Palavras.csv',index_col=0)
    words = set(df.sort_values(by='frequencia',ascending=False).head(15).index)

    #escolhendo as palavras que mais aparecem mas não possuem algum significado interessante apenas para deixar o grafo mais leve
    words.remove('não')


    df_f = df.drop(
        index=df.index.intersection(words),
        columns=df.columns.intersection(words),
    )

    to_graphLike(df_f,networkx=False)

    