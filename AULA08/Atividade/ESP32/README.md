# ESP32 - Guia de Firmware da AULA08

Este arquivo concentra as decisoes praticas de firmware para o projeto `Smart Desk Ergonomica IoT`. Use este guia junto com [../README.md](../README.md) para implementar, gravar e validar o comportamento da placa.

## 1. Bibliotecas

Bibliotecas esperadas no firmware:

- `WiFi.h`
- `WiFiClientSecure.h`
- `PubSubClient.h`
- `DFRobot_DHT11.h`
- `WebServer.h`
- biblioteca do display adotado pela dupla

Recomendacoes:

- para MQTT na porta `8883`, mantenha `WiFiClientSecure`;
- para o display, use a biblioteca compativel com o hardware real;
- se o display for um modulo `TM1637`, a biblioteca `TM1637Display.h` simplifica bastante a implementacao.

## 2. Pinagem do Hardware da Disciplina

Nao ha uma nova sugestao de pinagem para esta aula. O hardware base continua sendo o mesmo utilizado nas aulas anteriores, especialmente o conjunto apresentado em [AULA02](../../AULA02/README.md).

Pinos ja consolidados no repositorio:

| Funcao | GPIO | Referencia |
| --- | --- | --- |
| DHT11 `DATA` | `33` | [AULA02/leitura_dht11/leitura_dht11.ino](../../AULA02/leitura_dht11/leitura_dht11.ino) |
| LDR analogico | `39` | [AULA02/leitura_dht11/leitura_dht11.ino](../../AULA02/leitura_dht11/leitura_dht11.ino) |

Pinos definidos para a AULA08:

| Funcao | GPIO |
| --- | --- |
| Rele | `13` |
| Segmento `A` | `18` |
| Segmento `B` | `5` |
| Segmento `C` | `21` |
| Segmento `D` | `3` |
| Segmento `E` | `1` |
| Segmento `F` | `23` |
| Segmento `G` | `22` |
| Segmento `DP` | `19` |
| Display 1 | `16` |
| Display 2 | `17` |
| LED 1 | `4` |
| LED 2 | `0` |
| LED 3 | `2` |
| LED 4 | `15` |
| Botao 1 | `4` |
| Botao 2 | `0` |
| Botao 3 | `2` |
| Botao 4 | `15` |

Orientacao:

- use a mesma shield, os mesmos sensores e a mesma pinagem ja trabalhada nas aulas anteriores;
- os `4 botoes` usam os mesmos GPIOs dos `4 LEDs`, exigindo o mesmo cuidado com multiplexacao/logica de leitura e escrita adotado na `AULA07`;
- para qualquer componente adicional da atividade, siga o esquematico real do kit utilizado em laboratorio;
- nao documente pinos "inventados": registre apenas as conexoes efetivamente usadas pela dupla no hardware montado.

## 3. Parametros Configuraveis

Mantenha estes itens centralizados no inicio do codigo:

- `WIFI_SSID`
- `EAP_IDENTITY`
- `EAP_USERNAME`
- `EAP_PASSWORD`
- `GROUP_ID`
- `MQTT_HOST`
- `MQTT_PORT`
- intervalo de publicacao da telemetria
- limite de luminosidade para automacao
- limite de umidade para alerta
- duracao do ciclo de foco
- duracao da pausa

Valores base recomendados:

- `WIFI_SSID = "SATC 2.4"`
- `MQTT_HOST = "broker.hivemq.com"`
- `MQTT_PORT = 8883`

## 4. Topicos Esperados

Use um unico ponto de configuracao para gerar os topicos do grupo:

| Funcao | Topico |
| --- | --- |
| Temperatura | `satc/gX/telemetry/temperature` |
| Umidade | `satc/gX/telemetry/humidity` |
| Luminosidade | `satc/gX/telemetry/luminosity` |
| Comando da luminaria | `satc/gX/cmd/light` |
| Estado do ciclo | `satc/gX/status/focus` |
| Alertas do sistema | `satc/gX/status/alert` |

Troque `gX` pelo numero real do grupo antes do upload.

## 5. Blocos Funcionais do Codigo

Estruture o firmware em blocos simples e previsiveis:

1. `Setup de hardware`
   - configurar pinos de entrada e saida;
   - inicializar display, LEDs e rele em estado seguro.
2. `Conexao Wi-Fi`
   - usar `WiFi.disconnect(true)`;
   - manter `WiFi.mode(WIFI_STA)`;
   - conectar em `SATC 2.4` com `WPA2_AUTH_PEAP`.
3. `Conexao MQTT`
   - criar `WiFiClientSecure`;
   - aplicar `espClient.setInsecure()` para o laboratorio;
   - configurar `PubSubClient`, callback e reconexao.
4. `Leitura e publicacao`
   - ler `DHT11` e `LDR` em intervalos controlados;
   - publicar cada grandeza no topico correto;
   - publicar tambem `status/focus` e `status/alert`.
5. `Controle local`
   - ler botoes com debounce;
   - alternar foco, pausa, reset e override manual;
   - refletir tudo no display e nos LEDs.
6. `Controle remoto`
   - assinar `satc/gX/cmd/light`;
   - aceitar `ON` e `OFF`;
   - atualizar rele e estado local.
7. `HTTP WebServer`
   - subir o servidor apos o Wi-Fi conectar;
   - expor status atual e parametros principais;
   - responder rapidamente para facilitar testes.
8. `Loop principal`
   - manter reconexao Wi-Fi;
   - manter reconexao MQTT;
   - executar `mqttClient.loop()`;
   - atender `server.handleClient()`;
   - atualizar sensores, display e automacoes.

## 6. Roteiro de Upload

1. Instale as bibliotecas do ESP32 e as dependencias do projeto.
2. Abra o sketch no Arduino IDE ou no PlatformIO.
3. Preencha as credenciais WPA2 Enterprise.
4. Ajuste `GROUP_ID`, thresholds e tempos do ciclo.
5. Selecione a placa ESP32 correta.
6. Compile para garantir que as bibliotecas e pinos estao corretos.
7. Grave o firmware na placa.

## 7. Validacao Local

Na rede local e no Node-RED, voce deve conseguir confirmar:

- conexao com `SATC 2.4`;
- conexao com `broker.hivemq.com:8883`;
- publicacao das leituras de `temperature`, `humidity` e `luminosity`;
- recebimento de comandos `ON` e `OFF` em `satc/gX/cmd/light`;
- atualizacao dos estados `FOCUS`, `PAUSE`, `IDLE` e `ALERT`.

No navegador da rede local, valide:

- abertura da pagina `/`;
- exibicao das leituras atuais;
- exibicao do estado da luminaria;
- resposta da rota `/status`;
- mudanca refletida apos ajuste na rota `/config`, se implementada.

## 8. Troubleshooting

### Biblioteca ausente

- erro de compilacao com `No such file or directory` normalmente indica biblioteca nao instalada;
- confirme especialmente `PubSubClient`, `DFRobot_DHT11` e a biblioteca do display.

### Falha no Wi-Fi Enterprise

- revise `EAP_IDENTITY`, `EAP_USERNAME` e `EAP_PASSWORD`;
- confirme que a placa esta em `WIFI_STA`;
- teste novamente somente depois de limpar conexoes anteriores com `WiFi.disconnect(true)`.

### Host ou porta MQTT incorretos

- use `broker.hivemq.com` na porta `8883`;
- confirme que o cliente configurado eh `WiFiClientSecure`;
- se estiver usando a estrategia do laboratorio, nao esqueca `espClient.setInsecure()`.

### Topico ou payload nao correspondem

- use exatamente `satc/gX/cmd/light` para comando remoto;
- padronize `ON` e `OFF` como payloads aceitos;
- confirme que o `GROUP_ID` do firmware bate com o grupo usado no Node-RED.

### Node-RED nao recebe ou nao publica

- verifique se o dashboard esta inscrito nos mesmos topicos da placa;
- confirme se o fluxo usa o mesmo broker e a mesma porta compativel com a maquina;
- teste primeiro com um unico topico de telemetria antes de expandir o fluxo;
- use os `debug nodes` no Node-RED e a rota HTTP `/status` para localizar o ponto de falha.
