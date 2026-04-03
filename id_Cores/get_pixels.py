"""
Esse programa tem como objetivo salvar as informações de todos, ou até de 4000, pixels das imagens do folder escolhido, guardando essas informações em um arquivo
mais leve e mais facil de ler (para o python), que são arquivos .npy. Permitindo uma maior rapidez e menos gasto com memoria para as próximas funções.

Modificações que você deve fazer:

-Cole o caminho total do folder que contem as imagens para a variavel imagens_dir
- 





"""






import cv2
import os
import numpy as np




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
    

    #tabela com cada coluna representando uma imagem e cada linha dessa coluna representa um pixel, deixei um espaço de folga
    tabelona = np.zeros((4*(10**4),len(cam_imagens),3))

    #lista que guarda os nomes em ordem das imagens
    nomes_ordenados = np.empty((len(cam_imagens)),dtype = 'U30')
    
    



    for idx,img in enumerate(cam_imagens):
        #juntando o nome do arquivo da imagem para o caminho do folder das imagens
        caminho_img = os.path.join(imagens_dir,img)

        #salvar os nomes das imagens em uma lista ordenada
        nomes_ordenados[idx] = img

        #imagem 
        imagem = cv2.imread(caminho_img)

        #freio
        if i ==freio:
            break

        altura, largura,profundidade  = imagem.shape



        c = 0
        #itera sobre todos os pixels e guarda na lista
        for linha in range(altura):
            for coluna in range(largura):


                #se a quatidade de pixels for maior que o tamanho da tabela ele para
                if c >= 4*(10**4):
                    break

                #já aloca a informação na tabela
                tabelona[c,idx] = imagem[linha,coluna]
                c += 1
            
            if c >= 4*(10**4):
                break



        i+=1


    diretorio_pixels = '/home/lucca/Documents/Atividades escolares/impatech/Projeto portinari/Projeto-Portinari/Data/Pixels_data'

    #se o diretorio escolhido não existir então cria um diretorio
    if not os.path.exists(diretorio_pixels):
        print('O diretorio escolhido para guardar as informacoes não existe, entao uma pasta sera criado no diretorio em que o programa está')
        diretorio_pixels = os.path.join(os.getcwd(),'Pixels_data')
        os.mkdir(diretorio_pixels,exist_ok = True)

    #salva a array em um arquivo própio do numpy
    np.save(os.path.join(diretorio_pixels,'Tabela_Pixels'),tabelona)

    #salva a ordem das imagens
    np.save(os.path.join(diretorio_pixels,'Nomes_ordenados'),nomes_ordenados)



if __name__ == '__main__':
    ip = input('Tem certeza que quer rodar esse programa, é provavel que demore muito tempo [s/n]')

    if ip == 's':
        print('começando')
        get_pixels()
        print('O programa foi concluido')

