# 🚢 Radar Marítimo

![Radar Marítimo Banner](https://github.com/user-attachments/assets/aaa78436-055c-4b55-be8c-6c2bfae3a786)

> Uma reinterpretação moderna do clássico *Batalha Naval* — automatizado com botões e LEDs para proporcionar uma experiência interativa, acessível e divertida.

---

## 📑 Índice

- [🔍 Sobre](#-sobre)
- [📚 Documentação](#-documentação)
- [🚀 Como Executar](#-como-executar)
- [👥 Equipe](#-equipe)
- [📝 Licença](#-licença)

---

## 🔍 Sobre

**Radar Marítimo** é uma versão automatizada e interativa do tradicional jogo **Batalha Naval**, transformada com a ajuda do Arduino e programação em C++. O projeto visa tornar o jogo mais acessível, rápido e imersivo, utilizando componentes como **botões, LEDs, buzzers e displays LCD I2C**.

A proposta é eliminar as dificuldades de entendimento do jogo original — como coordenadas e marcações complexas — e permitir que qualquer pessoa jogue de forma simples, utilizando somente de **botões e leds**. As jogadas são processadas automaticamente, com **feedback visual e sonoro**, tornando a experiência mais dinâmica e educativa.

[Site](https://sites.google.com/d/1tSHFSn6spUILjA6oEXaHgu8bMZuuEwUy/p/1knj-OkKjhRbKZyLP4hsyLuhye7VeHsJT/edit )

---

## 📚 Documentação

### 🕹️ Como Funciona

1. **Inicialização**
   - Ao ligar o jogo, o display exibe uma mensagem de boas-vindas e indica que o **Jogador 1** deve iniciar.

2. **Posicionamento dos Barcos**
   - Cada jogador posiciona seus barcos (tamanhos 3, 2 e 1) escolhendo:
   - Posição inicial (via botões horizontais).
   - Direção (via botões vertical).
   - O display guia todo o processo com mensagens claras de qual barco deverá ser selecionado.
   - Logo após o posicionamento do **jogador 1** irá iniciar o posicionamento do **jogador 2** e tudo se repitirá

3. **Rodadas de Ataque**
   - Os jogadores se revezam atacando posições do tabuleiro adversário.
   - O sistema indica automaticamente:
   - **Acerto**: LED de cor correspondente ao jogador (verde ou rosa) + som de acerto + mensagem no display.
   - **Erro (água)**: LED azul + som de erro + mensagem no display.

4. **Condições de Vitória**
   - O jogo termina quando todos os barcos de um jogador forem **destruídos**.
   - O vencedor é anunciado no display, seguido de todos os leds ligarem verdes e reset do jogo/sistema.

---

## 🚀 Como Executar

### ✅ Bibliotecas necessárias

- Adafruit_NeoPixel.h
- LiquidCrystal_I2C.h

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

## 📝 Licença
