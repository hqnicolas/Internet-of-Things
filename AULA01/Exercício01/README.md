# Exercício 01: Controle de LEDs com Botão

Este projeto demonstra como controlar três LEDs (verde, amarelo e vermelho) utilizando um único botão. Ao pressionar o botão, o estado de todos os LEDs é invertido (ligados viram desligados e vice-versa). O sketch também lê um sensor analógico (como um LDR) e exibe o estado de todos os sensores e LEDs no Monitor Serial.

## Componentes Necessários

*   Placa de desenvolvimento (Ex: NodeMCU ESP8266)
*   1x LED Verde
*   1x LED Amarelo
*   1x LED Vermelho
*   1x Push Button (Botão de pulso)
*   1x Sensor Analógico (Ex: LDR)
*   Resistores apropriados para os LEDs e o sensor.
*   Protoboard e Jumpers

## Pinagem (Conexões)

| Componente                | Pino na Placa |
| ------------------------- | ------------- |
| LED Verde (LED_VD)        | `D0`          |
| LED Amarelo (LED_AM)      | `D3`          |
| LED Vermelho (LED_VM)     | `D4`          |
| Sensor Digital (Botão)    | `D5`          |
| Sensor Analógico (LDR)    | `A0`          |

**Nota:** O pino do sensor digital (`D5`) é configurado com `INPUT_PULLUP`, o que significa que o botão deve ser conectado entre o pino `D5` e o `GND`.

## Como Funciona

O estado de todos os pinos de entrada e saída (I/O) é gerenciado por uma `union` chamada `estados`. Essa estrutura de dados permite acessar o estado completo de 16 bits como uma única variável ou acessar campos de bits individuais para cada sensor e LED.

1.  **Leitura de Sensores:** A função `leituras_sensor()` lê o estado do botão e o valor do sensor analógico, atualizando a `union estados`.
2.  **Detecção de Pressionamento:** Na função `loop()`, o código verifica se o botão foi pressionado (detectando a transição de não pressionado para pressionado).
3.  **Inversão dos LEDs:** Se um pressionamento for detectado, os bits correspondentes aos três LEDs na `union estados` são invertidos.
4.  **Atualização Física:** A função `acende_leds()` é chamada para ler os estados da `union` e acender ou apagar os LEDs físicos de acordo.
5.  **Monitoramento Serial:** A cada ciclo, o estado atual dos sensores e LEDs é impresso no Monitor Serial para depuração e visualização.

## Para Executar

1.  Monte o circuito de acordo com a tabela de pinagem.
2.  Abra o arquivo `sketch.ino` na IDE do Arduino.
3.  Selecione a placa e a porta COM corretas.
4.  Faça o upload do código.
5.  Abra o Monitor Serial com a velocidade de 9600 baud para ver o status dos componentes.
6.  Pressione o botão para ligar e desligar os LEDs.