"""
Esse programa tem como objetivo salvar as informações de todos, ou até de 4000, pixels das imagens do folder escolhido, guardando essas informações em um arquivo
mais leve e mais facil de ler (para o python), que são arquivos .npy. Permitindo uma maior rapidez e menos gasto com memoria para as próximas funções.

Modificações que você deve fazer:

-Cole o caminho total do folder que contem as imagens para a variavel imagens_dir
- 

alguns problemas: se a função estiver em uma definição muito boa a imagem não pega os ultimos pixels 



"""






import cv2
import os
import numpy as np
import time


inicio = time.time()




def get_pixels(freio = -1):
    #o i é só para teste, freia o for das imagens
    i = 0

    
    imagens_dir = "/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/imagens"
    
    if not os.path.exists(imagens_dir):
        print(f"Erro: Diretório {imagens_dir} não encontrado!")
        return
    
    # cria uma lista com todos os caminhos
    cam_imagens = [f for f in os.listdir(imagens_dir) 
               if f.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp', '.gif'))]
    

    #diretorio que guardará a informação dos pixels
    diretorio_pixels = '/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/Pixels_data'


    #se o diretorio escolhido não existir então cria um diretorio
    if not os.path.exists(diretorio_pixels):
        print('O diretorio escolhido para guardar as informacoes não existe, entao uma pasta sera criado no diretorio em que o programa está')
        diretorio_pixels = os.path.join(os.getcwd(),'Pixels_data')
        os.mkdir(diretorio_pixels)



    #cria o arquivo numpy bom mesmo
    shape_tabelona = ((10**5),len(cam_imagens),3)

    #aqui o arquivo memap é um arquivo que permite trabalhar com arrays muito grandes sem guardalás na ram 
    tabelona = np.memmap(os.path.join(diretorio_pixels,'Tabelona_10e5.dat'),dtype='uint8', mode='w+', shape=shape_tabelona)

    #lista que guarda os nomes em ordem das imagens
    nomes_ordenados = np.empty((len(cam_imagens)),dtype = 'U30')
    tamanho_ordenados = np.empty((len(cam_imagens)),dtype = 'uint32')
    

    for idx,img in enumerate(cam_imagens):
        #juntando o nome do arquivo da imagem para o caminho do folder das imagens
        caminho_img = os.path.join(imagens_dir,img)

        #salvar os nomes das imagens em uma lista ordenada
        nomes_ordenados[idx] = img
        tamanho_ordenados[idx] = 0

        #imagem 
        imagem = cv2.imread(caminho_img)

        if imagem is not None:
            #freio
            if i ==freio:
                break

            #deixando a tabela das cores em uma tabela com apenas uma coluna
            imagem = imagem.reshape((-1,3))
            
            #tamanho da imagem
            tam,o = imagem.shape

            #a coluna tem um tamanho maximo 
            tamanho = min((10**5),tam)

            #guarda o tamanho util da coluna
            tamanho_ordenados[idx] = tamanho

            #embaralhando a lista dos pixels para não ter que pegar todos
            indices = np.random.choice(tam, tamanho, replace=False)

            #coloca a coluna no arquivo final
            tabelona[:tamanho,idx,:] = imagem[indices,:]

            i+=1

            tabelona.flush()


    #salva a ordem das imagens
    np.save(os.path.join(diretorio_pixels,'Nomes_ordenados'),nomes_ordenados)
    np.save(os.path.join(diretorio_pixels,'tamanhos_ordenados'),tamanho_ordenados)



if __name__ == '__main__':
    ip = input('Tem certeza que quer rodar esse programa, é provavel que demore muito tempo [s/n]')

    if ip == 's':
        print('começando')
        get_pixels()
        print('O programa foi concluido')

    tempo = time.time() - inicio
    print(f'O tempo gasta para esse programa foi de {tempo}')