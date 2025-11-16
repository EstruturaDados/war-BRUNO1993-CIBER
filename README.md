#  Projeto WAR Estruturado – Minha Implementação

Bem-vindo ao repositório da minha solução para oDesafio WAR Estruturado! Este projeto foi desenvolvido em C e implementa uma versão funcional do clássico jogo de estratégia, com foco em modularização, gerenciamento de memória e lógica de jogo.

A versão final neste repositório corresponde aoNível Mestre do desafio, que engloba todas as funcionalidades propostas.

---

 Demonstração em Vídeo

Veja o jogo em ação! A demonstração abaixo mostra a inicialização, um ataque bem-sucedido, uma tentativa de ataque inválida e a verificação da missão.

*(Aqui ficará o nosso GIF)*

---

 Como Compilar e Executar

Este projeto foi desenvolvido e testado em um ambiente Linux. Para compilar e executar, siga os passos abaixo:

1. Clone o repositório:
    bash
    git clone https://github.com/EstruturaDados/war-BRUNO1993-CIBER.git
    

2. Navegue até a pasta do projeto:
    bash
    cd war-BRUNO1993-CIBER
    

3. Compile o programa usando o GCC:
    bash
    gcc war.c -o war
    

4. Execute o jogo:
    bash
    ./war
    

---

 Funcionalidades da Versão Final

-Mapa Dinâmico: O mapa é criado comalocação dinâmica de memória (`calloc`).
-Batalhas Estratégicas: Lógica de batalha com rolagem de dados (`rand()`), ondeempates favorecem a defesa.
-Sistema de Missões: No início do jogo, uma missão secreta é atribuída ao jogador (Destruir o exército Vermelho ou Conquistar 7 territórios).
-Interface Interativa: Menu principal para atacar, verificar missão ou sair.
-Boas Práticas de Programação: Código modularizado, uso de ponteiros e `const` para segurança dos dados.

---
💡Desenvolvido por: Bruno Machado.