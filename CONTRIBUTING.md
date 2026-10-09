# Guia de Contribuição e Fluxo de Branches

Este documento define o padrão de desenvolvimento do Projeto Portinari para manter o repositório sincronizado e sem conflitos.

---

## Estrutura de Branches

* **`main`**: Branch principal. Contém o código estável e integrado.
* **Branches por frente ativa**:
  * `coleta`: Scripts de scraping e tratamento de obras.
  * `cores`: Extração e visualização de cores das pinturas.
  * `poemas`: Análise descritiva, processamento e classificador BERTimbau.
  * `duplicatas`: Scripts de detecção de inconsistências nos dados.

---

## Regras de Trabalho

1. **Escopo isolado:**
   Ao trabalhar na sua frente, modifique arquivos dentro da sua pasta correspondente em `frentes/<sua_frente>/`.
   
2. **Mudanças compartilhadas (`src/`, `dados/`, `README.md`):**
   Alterações na biblioteca comum ou em arquivos de dados compartilhados devem ser abertas via Pull Request direto para a `main`.

3. **Sincronização periódica (Semanal):**
   Para evitar defasagem com o trabalho dos demais membros, atualize sua branch com frequência:
   ```bash
   git checkout <sua_frente>
   git pull origin <sua_frente>
   git merge origin/main
   git push origin <sua_frente>
   ```

4. **Entrega de resultados (Pull Request):**
   Ao finalizar um experimento, notebook ou funcionalidade, abra um Pull Request da sua branch para a `main`.

---

## Gestão de Tarefas (Issues)

* Use o GitHub Issues para cadastrar to-dos e novas ideias.
* Ao commitar um resultado que resolve uma issue, cite o número no commit:
  ```bash
  git commit -m "cores: implementa extrator mean-shift (closes #5)"
  ```
