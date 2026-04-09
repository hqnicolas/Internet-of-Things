# Exercicio 2 - Webserver para LEDs da Shield

## Objetivo
Expandir o webserver sincronico do exercicio anterior para controlar os LEDs da shield ESP32 por meio de uma interface web.

## O que o programador deve fazer
1. Reaproveitar a base do servidor HTTP do exercicio 1.
2. Configurar os pinos dos LEDs da shield como saida:
   - `LED1 -> GPIO 4`
   - `LED2 -> GPIO 0`
   - `LED3 -> GPIO 2`
   - `LED4 -> GPIO 15`
3. Criar uma pagina HTML com botoes para ligar e desligar cada LED.
4. Criar rotas no servidor para receber os comandos enviados pela pagina.
5. Ao receber uma requisicao, alterar o estado do LED correspondente com `digitalWrite`.
6. Atualizar a pagina para refletir o estado atual dos LEDs, se desejar.
7. Manter o atendimento das requisicoes usando `server.handleClient()` no `loop()`.

## Requisitos minimos
- O servidor deve continuar respondendo na porta `80`
- A pagina principal deve exibir controles para os 4 LEDs
- Cada botao deve acionar uma rota diferente ou enviar parametros indicando:
  - qual LED sera controlado
  - se ele deve ligar ou desligar
- O hardware deve responder ao comando enviado pelo navegador

## Pinos usados
```cpp
#define LED1 4
#define LED2 0
#define LED3 2
#define LED4 15
```

## Sugestao de interface web
A pagina pode conter:
- um titulo como "Controle de LEDs"
- dois botoes por LED: `ON` e `OFF`
- links ou comandos JavaScript usando `fetch()`

## Exemplo de rotas possiveis
- `/` -> pagina HTML principal
- `/led1/on`
- `/led1/off`
- `/led2/on`
- `/led2/off`

Ou uma rota unica, por exemplo:
- `/led?num=1&state=on`

## Exemplo de comportamento esperado
- O usuario acessa a pagina pelo navegador
- Clica em um botao para ligar um LED
- O ESP32 recebe a requisicao e altera o GPIO correspondente
- O LED da shield acende ou apaga imediatamente

## Desafios extras opcionais
- Mostrar o estado atual de cada LED na pagina
- Criar um botao para ligar todos
- Criar um botao para desligar todos
- Melhorar o layout HTML com CSS simples

## Checklist
- O codigo compila sem erros
- Os quatro LEDs estao configurados como saida
- A pagina web exibe os controles
- Cada LED responde corretamente aos comandos
- O servidor continua funcional durante a interacao
