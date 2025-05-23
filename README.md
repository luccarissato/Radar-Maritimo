# Radar-Maritmo

<img src="imagem.png" alt="Exemplo imagem">

> Imagine o Batalha Naval, porem basicamente automatizado e facilitado para todas as pessoas que o podem jogar, com a utilizacao de botoes e LEDs a automatizacao deste classico ocorre!

## Índice

- [Sobre](#sobre)
- [Documentação](#documentação)
- [Equipe](#equipe)
- [Artigos Relacionados](#artigos-relacionados)


## Sobre
 O Radar Maritmo consiste no jogo Batalha Naval, porem reimaginado e feito de maneira Figital, modificado utilizando o Arduino, C++, transformando o processo analogico e lento de uma batalha naval comum em um processo rapido que
 ocorre na velocidade de dois cliques de dois butoes, e automaticamente o sistema verifica caso o inimigo tenha sido atingido ou nao sendo o jogador notificado pelo buzzer, este projeto tem o objetivo de facilitar o entendimento
 do jogo Batalha Naval que normalmente assustaria as pessoas que teriam o interesse de ver o jogo porem as varias marcas, entender como avisar seu inimigo onde voce deseja acertar baseado nas coordenadas, completamente simplificado no Radar Maritimo.


## Documentação
1. Ao Iniciar o Jogo as duas "telas" de LED do jogador 1 e 2, brilham se movendo angularmente em duas cores diferentes dependendo de qual lado esta o jogador, anunciando a iniciacao do jogo Radar Maritimo.

2. No proximo "estagio" os jogadores devem escolher em qual posicao seus respectivos barcos devem ser colocados, sendo o barco que ocupa 3 espacos, 2 espacos e 1 espaco respectivamente, e em cada estagio da escolha de posicao dos barcos o jogador escolhe se o barco se direciona verticalmente ou horizontalmente e em quais posicoes adjacentes ele pode ser colocado, e em cada parte das escolhas uma mensagem vai ser mostrada no display(caso o jogador deseje cancelar sua acao passada, deve apertar no botao A 2 vezes rapidamente).

3. Apos as escolhas dos barcos de ambos os jogadores o verdadeiro jogo comeca onde as rodadas ocorrem em quais ambos os jogadores tentam acertar e eliminar os barcos inimigos, o qual ocorre de maneira similar a escolha dos barcos, e dependendo se acertou um pedaco do barco, a agua ou destruiu completamente um barco, a cor do LED muda juntamente ao som programado de um buzzer e o display, para indicar e mostrar qual foi o resultado da acao do jogador.

4. Sendo o "Loop" de gameplay sendo o apresentado anteriormente, o final do jogo e chegado apos todos os barcos de algum dos dois jogadores forem em sua totalidade acertados, assim uma tela de vitoria aparecera para o jogador vencedor e uma tela de ”Bom jogo, quer jogar novamente?” Para o perdedor, e apos alguns momentos a tela de LEDs retornara para a configuracao inicial antes das modificacoes feitas nas partidas.



## Equipe 

|                 **Igor Gabriel**                  |                    **Lucca Rissato**                    |                   **Daniel dos Santos**                   |          
| :------------------------------------------------: | :------------------------------------------------------: | :--------------------------------------------------: | 
| ![Igor](https://avatars.githubusercontent.com/u/107767224?v=4) | ![Lucca](https://avatars.githubusercontent.com/luccarissato) | ![Daniel](https://avatars.githubusercontent.com/u/210912757?v=4) | 
|           **Displays e Logica De Jogo**           |              **Leds e Logica De Jogo**               |     **Sonoplastia**      |             

Veja a lista de [colaboradores](https://github.com/luccarissato/Radar-Maritimo/graphs/contributors).

## Artigos relacionados
[Logica](https://www.canva.com/design/DAGmTMpjgZU/M6ENvyQbRjVcPlpwgezG6w/edit)

[LEDs](https://docs.google.com/document/d/1Sy9DZSqPzpIKdoNRvKYAcvU3oX75fMmVJiSy4DJXtCE/edit?tab=t.0)

[Rascunho Da Logica](https://docs.google.com/document/d/1cCqNBBMvbgPbsKnjYv-kbeMVeqmdhjC4WygrdKPP7Q8/edit?tab=t.0)

