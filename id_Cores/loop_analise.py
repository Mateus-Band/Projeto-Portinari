import cv2
import os
import random
import numpy as np
from PIL import Image, ImageDraw, ImageFont
from detectar_cores2 import most_collor2, how_color2

import sys
diretorio_atual = os.path.dirname(os.path.abspath(__file__))
diretorio_pai = os.path.dirname(diretorio_atual)
if diretorio_pai not in sys.path:
    sys.path.append(diretorio_pai)


def get_cor_media(caminho_imagem, n_pixels=100):
    """Extrai a cor média da imagem sem prints"""
    imagem = cv2.imread(caminho_imagem)
    if imagem is None:
        return None
    
    b_sum = g_sum = r_sum = 0
    for i in range(n_pixels):
        pixel = list(imagem[int(random.uniform(0, imagem.shape[0])), 
                            int(random.uniform(0, imagem.shape[1]))])
        b_sum += int(pixel[0])
        g_sum += int(pixel[1])
        r_sum += int(pixel[2])
    
    return [r_sum/n_pixels, g_sum/n_pixels, b_sum/n_pixels]

def criar_grid_analise(num_imagens=20, tamanho_celula=250, output_path="grid_analise.png"):
    """
    Cria um grid com análise de cores de imagens aleatórias.
    
    Args:
        num_imagens: Quantidade de imagens a analisar
        tamanho_celula: Tamanho de cada célula do grid em pixels
        output_path: Caminho para salvar a imagem final
    """
    
    # Caminho das imagens
    imagens_dir = "../Data/imagens"
    
    if not os.path.exists(imagens_dir):
        print(f"Erro: Diretório {imagens_dir} não encontrado!")
        return
    
    # Listar todas as imagens
    imagens = [f for f in os.listdir(imagens_dir) 
               if f.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp', '.gif'))]
    
    if len(imagens) < num_imagens:
        print(f"Aviso: Apenas {len(imagens)} imagens encontradas. Usando todas.")
        num_imagens = len(imagens)
    
    # Selecionar imagens aleatórias
    imagens_selecionadas = random.sample(imagens, num_imagens)
    print(f"Analisando {len(imagens_selecionadas)} imagens...")
    
    # Configurar grid
    colunas = 4
    linhas = (num_imagens + colunas - 1) // colunas
    altura_titulo = 30
    altura_texto = 150
    tamanho_imagem = tamanho_celula - altura_texto - altura_titulo
    
    # Dimensões da imagem final
    largura_final = colunas * tamanho_celula
    altura_final = linhas * tamanho_celula
    
    # Criar imagem branca
    grid_image = Image.new('RGB', (largura_final, altura_final), color='white')
    draw = ImageDraw.Draw(grid_image)
    
    # Tentar carregar uma fonte, se não conseguir usa a padrão
    try:
        fonte_titulo = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 11)
        fonte = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 9)
    except:
        fonte_titulo = ImageFont.load_default()
        fonte = ImageFont.load_default()
    
    # Processar cada imagem
    for idx, nome_img in enumerate(imagens_selecionadas):
        caminho_img = os.path.join(imagens_dir, nome_img)
        
        # Calcular posição na grid
        linha = idx // colunas
        coluna = idx % colunas
        x_inicio = coluna * tamanho_celula
        y_inicio = linha * tamanho_celula
        
        try:
            # Desenhar título (nome do arquivo)
            draw.rectangle([x_inicio, y_inicio, x_inicio + tamanho_celula, 
                          y_inicio + altura_titulo], fill=(200, 200, 200))
            
            # Truncar nome se for muito longo
            nome_exibicao = nome_img if len(nome_img) <= 35 else nome_img[:32] + "..."
            draw.text((x_inicio + 5, y_inicio + 5), nome_exibicao, fill='black', font=fonte_titulo)
            
            # Carregar e redimensionar imagem
            img = Image.open(caminho_img).convert('RGB')
            img.thumbnail((tamanho_imagem, tamanho_imagem), Image.Resampling.LANCZOS)
            
            # Calcular posição centralizada
            x_pos = x_inicio + (tamanho_celula - img.width) // 2
            y_pos = y_inicio + altura_titulo + (tamanho_imagem - img.height) // 2
            
            # Colar imagem no grid
            grid_image.paste(img, (x_pos, y_pos))
            
            # Extrair informações de cor
            print(f"  [{idx+1}/{len(imagens_selecionadas)}] Processando {nome_img}...", end="")
            
            cor_media = get_cor_media(caminho_img)
            cor_principal = most_collor2(caminho_img, n_pixels=5000)
            
            # Preparar texto
            texto = f"Média: {cor_media[0]:.0f},{cor_media[1]:.0f},{cor_media[2]:.0f}\nPrincipal: {cor_principal}"
            
            # Desenhar fundo para texto (retângulo cinza)
            y_texto = y_inicio + altura_titulo + tamanho_imagem + 5
            draw.rectangle([x_inicio, y_texto, x_inicio + tamanho_celula, 
                          y_inicio + tamanho_celula], fill=(240, 240, 240))
            
            # Desenhar texto
            draw.text((x_inicio + 5, y_texto + 5), texto, fill='black', font=fonte)
            
            print(" ✓")
            
        except Exception as e:
            print(f" ✗ Erro: {e}")
            # Desenhar X em caso de erro
            draw.text((x_inicio + 10, y_inicio + 10), "Erro ao\ncarregar", fill='red', font=fonte)
    
    # Salvar imagem
    grid_image.save(output_path)
    print(f"\n✓ Grid salvo em: {output_path}")
    print(f"  Dimensões: {largura_final}x{altura_final} pixels")
    return grid_image

if __name__ == '__main__':
    # Criar grid com 20 imagens
    criar_grid_analise(num_imagens=20, tamanho_celula=300, output_path="analise_cores_20.png")