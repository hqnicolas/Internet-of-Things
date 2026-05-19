# Projeto: Smart Desk Ergonomica IoT

**Foco:** produtividade, saude ocupacional e eficiencia energetica.

Este projeto transforma uma mesa de trabalho comum em um sistema inteligente que monitora o ambiente, gerencia ciclos de concentracao e automatiza a iluminacao. A comunicacao principal usa o broker publico `broker.hivemq.com`, reaproveitando o padrao de rede e MQTT apresentado na [AULA07](../../AULA07/README.md) e detalhado no firmware em [ESP32/README.md](./ESP32/README.md).

## 1. Objetivo Operacional

Ao final da atividade, a dupla deve conseguir:

- ler `temperatura`, `umidade` e `luminosidade` no ESP32;
- publicar os dados no broker MQTT;
- mostrar os dados no dashboard do Node-RED;
- controlar a luminaria por comando remoto;
- executar pelo menos uma automacao;
- disponibilizar um `HTTP WebServer` para consulta local do estado do sistema.

Observacao:

- o projeto ja cobre tres grandezas monitoradas;
- se o professor exigir tres sensores fisicos distintos, adicione um terceiro modulo e publique a nova leitura com o mesmo padrao dos topicos abaixo.

## 2. Checklist Inicial

Antes de comecar, confirme que a dupla tem:

- `1 ESP32`;
- `1 DHT11`;
- `1 LDR` com resistor para divisor de tensao;
- `4 botoes`;
- `1 rele` para a luminaria;
- `4 LEDs` de status com resistores;
- `1 conjunto de display` para exibir `00` a `99`;
- protoboard, jumpers e alimentacao;
- Arduino IDE ou PlatformIO configurado para ESP32;
- bibliotecas `WiFi.h`, `WiFiClientSecure.h`, `PubSubClient.h`, `DHT.h`, `WebServer.h` e a biblioteca do display escolhido;
- acesso ao Wi-Fi `SATC 2.4` com credenciais WPA2 Enterprise;
- Node-RED funcionando na maquina de desenvolvimento;
- acesso ao broker `broker.hivemq.com`.

## 3. Arquitetura do Projeto

### Entradas

- `DHT11`: temperatura e umidade relativa do ar;
- `LDR`: luminosidade sobre a mesa;
- `Botao Start/Pause`: inicia ou pausa o cronometro;
- `Botao Reset`: reinicia o ciclo atual;
- `Botao Toggle Display`: alterna a informacao exibida;
- `Botao Manual Override`: liga ou desliga a iluminacao manualmente.

### Saidas

- `Display`: exibicao de valores de `00` a `99`;
- `Rele`: acionamento da luminaria;
- `LED 1`: modo foco ativo;
- `LED 2`: pausa em andamento;
- `LED 3`: conexao Wi-Fi/MQTT ativa;
- `LED 4`: alerta ambiental.

### Papel de cada camada

- `ESP32`: leitura dos sensores, botoes, regras locais, MQTT e HTTP;
- `Broker MQTT`: troca de mensagens entre placa e Node-RED;
- `Node-RED`: dashboard, automacoes, historico e comandos remotos.

## 4. Convencoes de Comunicacao

### Wi-Fi

O ESP32 deve se conectar ao Wi-Fi institucional `SATC 2.4` usando `WPA2 Enterprise`, como no exemplo da aula 07.

### MQTT

Configuracao recomendada:

- host: `broker.hivemq.com`
- porta: `8883`
- cliente: `WiFiClientSecure`
- estrategia de laboratorio: `espClient.setInsecure()`

Padrao de topicos por grupo:

| Funcao | Topico | Direcao | Payload esperado |
| --- | --- | --- | --- |
| Temperatura | `satc/gX/telemetry/temperature` | ESP32 -> Node-RED | valor numerico em `C` |
| Umidade | `satc/gX/telemetry/humidity` | ESP32 -> Node-RED | valor numerico em `%` |
| Luminosidade | `satc/gX/telemetry/luminosity` | ESP32 -> Node-RED | valor numerico bruto ou percentual |
| Comando da luminaria | `satc/gX/cmd/light` | Node-RED -> ESP32 | `ON` ou `OFF` |
| Estado do ciclo | `satc/gX/status/focus` | ESP32 -> Node-RED | `IDLE`, `FOCUS` ou `PAUSE` |
| Alertas do sistema | `satc/gX/status/alert` | ESP32 -> Node-RED | `OK`, `WARN` ou `ALERT` |

Substitua `gX` pelo numero do grupo, por exemplo `g7`.

Se a dupla quiser reaproveitar o modelo global da aula 07 para testes extras, pode adicionar topicos complementares como `satc/gX/cmd/all` ou `satc/all/all`, mas eles nao sao obrigatorios para esta atividade.

### HTTP WebServer

O segundo protocolo da atividade sera `HTTP WebServer`.

Uso recomendado:

- pagina raiz `/` com resumo de sensores, estado do foco e estado da luminaria;
- rota `/status` para consulta rapida do estado atual;
- rota `/config` para ajuste de parametros simples, como tempo de foco e limite de luminosidade.

## 5. Etapas de Execucao

### 1. Montar o circuito

- conecte o `DHT11`, o `LDR`, os `4 botoes`, o `rele`, os `4 LEDs` e o display ao ESP32;
- confira alimentacao, `GND` comum e resistores dos LEDs e do divisor do LDR;
- escolha uma pinagem consistente e registre essa escolha no relatorio.

Use a sugestao de firmware em [ESP32/README.md](./ESP32/README.md) para reduzir retrabalho.

### 2. Configurar o Wi-Fi WPA2 Enterprise

- copie para o firmware os campos `WIFI_SSID`, `EAP_IDENTITY`, `EAP_USERNAME` e `EAP_PASSWORD`;
- mantenha `WiFi.mode(WIFI_STA)` e o `WiFi.begin(...)` no padrao da aula 07;
- valide a conexao do ESP32 antes de prosseguir para o MQTT.

### 3. Configurar o MQTT

- defina `MQTT_HOST = "broker.hivemq.com"` e `MQTT_PORT = 8883`;
- inicialize `WiFiClientSecure` e `PubSubClient`;
- configure o `GROUP_ID` da dupla e monte os topicos do grupo;
- publique primeiro uma mensagem de teste para garantir que a conexao com o broker esta correta.

### 4. Implementar a telemetria dos sensores

- leia `temperatura` e `umidade` pelo `DHT11`;
- leia `luminosidade` pela entrada analogica do `LDR`;
- publique cada leitura em seu proprio topico;
- normalize os intervalos de publicacao para evitar spam no broker;
- reflita estados importantes no display e nos LEDs locais.

### 5. Implementar comandos remotos para rele e LEDs

- assine pelo menos o topico `satc/gX/cmd/light`;
- trate payloads `ON` e `OFF`;
- acione o `rele` e atualize a sinalizacao local quando um comando for recebido;
- mantenha o `Manual Override` coerente com o ultimo estado aplicado.

### 6. Expor o HTTP WebServer

- inicie o servidor HTTP somente depois do Wi-Fi estar conectado;
- mostre na pagina raiz as leituras correntes e os estados `FOCUS`, `PAUSE` ou `IDLE`;
- permita consultar rapidamente se a luminaria esta ligada e qual limite de luminosidade esta ativo;
- se usar rota de configuracao, aplique validacao simples e reflita as alteracoes no estado HTTP e no Node-RED.

### 7. Criar o dashboard e as automacoes no Node-RED

- use `mqtt in` para `temperature`, `humidity`, `luminosity`, `focus` e `alert`;
- monte gauges para temperatura e umidade;
- monte grafico ou indicador para luminosidade;
- adicione botao ou switch para publicar `ON` e `OFF` em `satc/gX/cmd/light`;
- implemente pelo menos uma automacao, como:
  - se a luminosidade cair abaixo do limite e o estado for `FOCUS`, publicar `ON` para a luminaria;
  - se a umidade ficar abaixo de `30`, publicar ou exibir um alerta visual.

### 8. Validar a comunicacao bidirecional

- altere a luminosidade sobre o LDR e confira a atualizacao no Node-RED;
- confirme que comandos enviados pelo dashboard acionam o rele na placa;
- verifique se os LEDs de status refletem modo foco, pausa, conexao e alerta;
- acesse a pagina HTTP do ESP32 pelo navegador e confira o mesmo estado visto no dashboard;
- simule queda e retorno de conexao para validar reconexao de Wi-Fi e MQTT, se esse diferencial estiver implementado.

## 6. Resultado Esperado

Ao final da execucao, o sistema deve:

- monitorar o ambiente da mesa em tempo real;
- exibir as leituras no Node-RED;
- permitir controle manual pelo dashboard;
- automatizar a iluminacao com base nas regras definidas;
- manter uma pagina HTTP local com o estado do sistema;
- demonstrar claramente o fluxo `ESP32 -> MQTT -> Node-RED -> MQTT -> ESP32`.

## 7. Checklist de Entrega

- firmware do ESP32 organizado e comentado;
- topicos MQTT documentados com o numero correto do grupo;
- fluxo exportado do Node-RED em `.json`;
- evidencias de teste no dashboard e no HTTP WebServer;
- relatorio tecnico com arquitetura, implementacao, testes e comparacao entre MQTT e HTTP WebServer.
