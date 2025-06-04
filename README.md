# 🚢 Radar Marítimo

![Radar Marítimo Banner](https://github.com/user-attachments/assets/aaa78436-055c-4b55-be8c-6c2bfae3a786)

> Uma reinterpretação moderna do clássico *Batalha Naval* — automatizado com botões e LEDs para proporcionar uma experiência interativa, acessível e divertida.

---

## 📑 Índice

- [🔍 Sobre](#-sobre)
- [📚 Documentação](#-documentação)
- [🚀 Como Executar](#-como-executar)
- [👥 Equipe](#-equipe)
- [📎 Artigos Relacionados](#-artigos-relacionados)
- [📝 Licença](#-licença)

---

## 🔍 Sobre

**Radar Marítimo** é uma versão automatizada e interativa do tradicional jogo **Batalha Naval**, transformada com a ajuda do Arduino e programação em C++. O projeto visa tornar o jogo mais acessível, rápido e imersivo, utilizando componentes como **botões, LEDs, buzzers e displays LCD**.

A proposta é eliminar as barreiras de entendimento do jogo original — como coordenadas e marcações complexas — e permitir que qualquer pessoa jogue de forma simples, utilizando apenas dois botões. As jogadas são processadas automaticamente, com **feedback visual e sonoro**, tornando a experiência mais dinâmica e educativa.

---

## 📚 Documentação

### 🕹️ Como Funciona

1. **Inicialização**
   - Ao ligar o jogo, o display exibe uma mensagem de boas-vindas e indica que o **Jogador 1** deve iniciar.

2. **Posicionamento dos Barcos**
   - Cada jogador posiciona seus barcos (tamanhos 3, 2 e 1) escolhendo:
     - Direção (horizontal ou vertical).
     - Posição inicial (via botões).
   - O display guia todo o processo com mensagens claras.

3. **Rodadas de Ataque**
   - Os jogadores se revezam atacando posições no tabuleiro adversário.
   - O sistema indica automaticamente:
     - **Acerto**: LED de cor correspondente + som de acerto + mensagem no display.
     - **Erro (água)**: LED diferente + som específico + mensagem no display.

4. **Condições de Vitória**
   - O jogo termina quando todos os barcos de um jogador forem destruídos.
   - O vencedor é anunciado no display, seguido de uma animação e reset visual do sistema.

---

## 🚀 Como Executar

### ✅ Bibliotecas necessárias

- Neopixel
- 

### 📦 Instalação

1. Clone o repositório:
   ```bash
   git clone https://github.com/luccarissato/Radar-Maritimo.git
2. Abra o arquivo .ino na IDE do Arduino.

3. Conecte seu Arduino via USB e selecione a placa e porta correta.

4. Faça o upload do código para o Arduino.


## 👥 Equipe 

|                 **Igor Gabriel**                  |                    **Lucca Rissato**                    |                   **Daniel dos Santos**                   |          
| :------------------------------------------------: | :------------------------------------------------------: | :--------------------------------------------------: | 
| ![Igor](https://avatars.githubusercontent.com/u/107767224?v=4) | ![Lucca](https://avatars.githubusercontent.com/luccarissato) | ![Daniel](https://avatars.githubusercontent.com/u/210912757?v=4) | 
|           **Displays e Logica De Jogo**           |              **Leds e Logica De Jogo**               |     **Sonoplastia**      |             

Veja a lista de [colaboradores](https://github.com/luccarissato/Radar-Maritimo/graphs/contributors).

## Artigos relacionados
[🧠 Logica](https://www.canva.com/design/DAGmTMpjgZU/M6ENvyQbRjVcPlpwgezG6w/edit)

[💡 LEDs](https://docs.google.com/document/d/1Sy9DZSqPzpIKdoNRvKYAcvU3oX75fMmVJiSy4DJXtCE/edit?tab=t.0)

[📝 Rascunho Da Logica](https://docs.google.com/document/d/1cCqNBBMvbgPbsKnjYv-kbeMVeqmdhjC4WygrdKPP7Q8/edit?tab=t.0)

## Licença
