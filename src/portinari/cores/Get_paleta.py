import numpy as np
import os
from Bin_method import Binacao,Bin_method,media_Bin


def get_tabelona(tamanhos_path,nomes_path,tabelona_path,tabelona_name = 'Tabelona_10e5.dat'):
    #abrindo os arquivos com as informações das obras
    #tamanhos ordenados
    tams_orde = np.load('/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/Pixels_data/tamanhos_ordenados.npy', mmap_mode='r')
    #nomes ordenados das obras
    nomes_orde = np.load('/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/Pixels_data/Nomes_ordenados.npy', mmap_mode='r')

    #diretório onde os pixels estão
    diretorio_pixels = '/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/Pixels_data'

    #tabela em que cada coluna possue os pixels de uma imagem especifica
    tabelona = np.memmap(os.path.join(diretorio_pixels,tabelona_name),dtype='uint8', mode='r',shape = (max(tams_orde),1575,3))
    _,colunas_tab,_ = tabelona.shape


    if (colunas_tab != len(tams_orde)) or (colunas_tab != len(nomes_orde)):
        print('erro, temos numeros diferentes de imagens, nomes das imagens e tamanhos das imagens')
        #deve parar aqui





#numero de imagens
numero_de_imagen = 1575




#loop sobre as colunas da tabelona, que são as imagens
for idx in range(10):
    img = tabelona[idx]

    #pré quantização dos pixels
    img = img//8

    clusters = Bin_method(img,5)
    paleta = media_Bin(clusters)
    
    ls = []
    for cor in paleta:
        ls.append(tuple(cor.tolist()))

    print(f'A imagem {nomes_orde[idx]} gerou a paleta: {ls}', end='\n\n')
