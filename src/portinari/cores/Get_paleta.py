import numpy as np
import os
from .Bin_method import Binacao, Bin_method, media_Bin
from portinari import caminhos


def get_tabelona(tamanhos_path=None, nomes_path=None, tabelona_dir=None, tabelona_name='Tabelona_10e5.dat'):
    diretorio_pixels = tabelona_dir or os.environ.get("PORTINARI_PIXELS_DIR", str(caminhos.DADOS_DIR / "Pixels_data"))
    t_path = tamanhos_path or os.path.join(diretorio_pixels, 'tamanhos_ordenados.npy')
    n_path = nomes_path or os.path.join(diretorio_pixels, 'Nomes_ordenados.npy')

    #abrindo os arquivos com as informações das obras
    #tamanhos ordenados
    tams_orde = np.load(t_path, mmap_mode='r')
    #nomes ordenados das obras
    nomes_orde = np.load(n_path, mmap_mode='r')

    #tabela em que cada coluna possue os pixels de uma imagem especifica
    tabelona = np.memmap(os.path.join(diretorio_pixels, tabelona_name), dtype='uint8', mode='r', shape=(max(tams_orde), 1575, 3))
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
