# Exercicio 3 - Webserver informando valores dos sensores

## Objetivo
Criar um webserver sincronico no ESP32 capaz de ler os sensores da shield e apresentar seus valores em uma pagina web.

Neste exercicio, o ESP32 deve agir como servidor web e tambem como ponto de acesso Wi-Fi, permitindo conexao direta do navegador ao dispositivo.

## O que o programador deve fazer
1. Reaproveitar a estrutura do servidor criada nos exercicios anteriores.
2. Configurar a leitura dos sensores disponiveis na shield.
3. Ler periodicamente os valores dos sensores no ESP32.
4. Exibir esses valores em uma pagina HTML servida pelo proprio ESP32.
5. Organizar as rotas para que o navegador possa consultar os dados.

## Sensores sugeridos
- `DHT11 -> GPIO 33`
  - informar temperatura e umidade
- `LDR -> GPIO 39`
  - informar nivel de luminosidade
- `Potenciometro -> GPIO 34`
  - informar valor analogico lido

## Requisitos minimos
- O ESP32 deve operar em modo `WIFI_AP`
- O servidor deve possuir uma pagina principal acessivel pelo navegador
- A pagina deve mostrar os valores atuais dos sensores
- Os valores devem ser atualizados a cada novo acesso ou por atualizacao automatica da pagina
- A leitura dos sensores deve acontecer de forma confiavel

## Bibliotecas que podem ser usadas
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
```

## Sugestao de implementacao
O programador pode seguir uma destas abordagens:

1. Atualizacao a cada carregamento da pagina
   - a rota `/` monta o HTML ja com os valores atuais

2. Pagina HTML + rota de dados
   - `/` retorna a pagina HTML
   - `/sensores` retorna os dados em texto ou JSON
   - JavaScript no navegador atualiza os valores periodicamente

## Exemplo de dados exibidos
- Temperatura: `27 C`
- Umidade: `65 %`
- Luminosidade: `78 %`
- Potenciometro: `2048`

## Exemplo de comportamento esperado
- O ESP32 cria sua propria rede Wi-Fi e inicia o servidor
- O usuario acessa a pagina pelo navegador
- A pagina mostra os valores atuais dos sensores da shield
- Ao mudar a luz ambiente ou girar o potenciometro, os valores exibidos mudam

## Desafios extras opcionais
- Atualizar os dados automaticamente a cada 1 segundo
- Exibir os sensores em formato JSON em uma rota separada
- Mostrar aviso caso a leitura do DHT falhe
- Aplicar conversao de escala para percentual no LDR

## Checklist
- O codigo compila sem erros
- O ESP32 cria a rede Wi-Fi corretamente
- O servidor responde no navegador
- O DHT11 retorna temperatura e umidade
- O LDR responde a variacao de luz
- O potenciometro altera os valores lidos
- A pagina apresenta os dados de forma clara
