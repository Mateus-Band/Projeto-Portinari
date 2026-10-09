'''
Esse arquivo contem a função cor_predominante, que usa o algoritimo Mean Shift para retornar uma paleta em ordem de proporção em relação a imagem.


'''



import numpy as np
from sklearn.cluster import MeanShift
import os





def cor_predominante(Dados):
    '''
    Usando o algoritmo Mean Shift implementado no sklearn iremos calcular os clusters, e retornar o cluster com mais pontos e a porcentagem de pontos totais.
    
    
    '''

    normed_data = Dados/255


    #h é o raio da bola, qeu é escolhida arbitrariamente
    h = 0.3

    #criação do objto ms, para calculo dos clusters  
    ms = MeanShift(bandwidth=h, bin_seeding=True, n_jobs=-1)

    #calcula os centroides
    ms.fit(normed_data)


    cluster_center = ms.cluster_centers_
    moda = np.argmax(np.bincount(ms.labels_))

    proporcao = len(ms.labels_[ms.labels_ == moda])/len(ms.labels_)

    return cluster_center,proporcao


if __name__ == '__main__':

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
    for idx in range(4):
        img = tabelona[idx]

        print(img.shape)

        cor,qtd = cor_predominante(img)

        print(f'A imagem {nomes_orde[idx]} tem como cores principais as com coordenadas : {cor}, com proporcao {qtd}')
