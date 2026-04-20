import cv2
import random
import numpy as np
import os
from sklearn.cluster import KMeans

#função para calcula da inercia do método do cotovelo
def inertias(Data):
    """
    Aqui basicamente vamos calcular o kmeans para todos os clusters, e calcular a inertia de cada um.
    Vamos retornar uma lista com as inertias e com as coordenadas dos centros, pois com o centros não precisamos re iterar para encontrar os clusters. 
    """
    img = Data
    inertias_list = []

    for n_cluster in range(2,6):
        kmeans = KMeans(n_clusters=n_cluster, random_state=0, n_init="auto",max_iter=50).fit(img)
        inertias_list.append(kmeans.inertia_)
        
    return inertias_list



def optimal_number_of_clusters(Data):
    """
    Vamos calcular o ponto mais dinstante entre a função inertia por cluster e a reta formada pelos pontos (1 cluster, inertia) e (ultimo cluster, inertia)
    
    Ao fim retornar o indice do melhor numero de cluster, e uma lista com os centros calculados para aquele numero de clusters. 
    """
    #inertias
    inertias_list= inertias(Data)




    x0,y0 = 2,inertias_list[0]
    x1,y1 = 5,inertias_list[-1]

    distancias =[]

    for i in range(len(inertias_list)):
        #ponto inicial
        x = i + 2
        y = inertias_list[i]
        
        #calculo matemático da distância
        numerador = abs((y1-y0)*x - (x1 - x0)*y + x1*y0 - y1*x0)
        denominador = np.sqrt((y1 - y0)**2 + (x1 - x0)**2)
        
        distancias.append(numerador/denominador)

    return distancias.index(max(distancias)) + 2





#numero de imagens
numero_de_imagen = 1575

#abrindo os arquivos com as informações das obras
#tamanhos ordenados
tams_orde = np.load('/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/Pixels_data/tamanhos_ordenados.npy', mmap_mode='r')
#nomes ordenados das obras
nomes_orde = np.load('/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/Pixels_data/Nomes_ordenados.npy', mmap_mode='r')

#diretório onde os pixels estão
diretorio_pixels = '/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/Pixels_data'

#tabela em que cada coluna possue os pixels de uma imagem especifica
tabelona = np.memmap(os.path.join(diretorio_pixels,'Tabelona_10e5.dat'),dtype='uint8', mode='r',shape = (max(tams_orde),1575,3))
_,colunas_tab,_ = tabelona.shape


if (colunas_tab != len(tams_orde)) or (colunas_tab != len(nomes_orde)):
    print('erro, temos numeros diferentes de imagens, nomes das imagens e tamanhos das imagens')
    #deve parar aqui


#loop sobre as colunas da tabelona, que são as imagens
for idx in range(10):
    img = tabelona[idx]


    #método do cotovelo para descobrir a quantidade de clusters
    n_cluster = optimal_number_of_clusters(img)
    kmeans = KMeans(n_clusters= n_cluster, random_state=0, n_init="auto",).fit(img)


    centroides = []
    for cluster in kmeans.cluster_centers_:
        centroides.append(tuple(cluster))

    print(f'Foi retirado a paleta: {centroides} da imagem {nomes_orde[idx]}')

