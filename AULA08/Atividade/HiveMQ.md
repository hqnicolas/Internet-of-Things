# Configuracao do HiveMQ e Topicos MQTT

## Broker MQTT da atividade

Use o broker publico do HiveMQ:

- **Host:** `broker.hivemq.com`
- **Porta MQTT TLS:** `8883`
- **Protocolo:** `MQTT`
- **Autenticacao:** nao obrigatoria para este teste
- **TLS:** habilitado

## Configuracao no ESP32

No firmware, mantenha:

```cpp
const char *MQTT_HOST = "broker.hivemq.com";
constexpr int MQTT_PORT = 8883;

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);
```

Como o broker usa TLS e a atividade esta em ambiente de laboratorio:

```cpp
espClient.setInsecure();
mqttClient.setServer(MQTT_HOST, MQTT_PORT);
```

## Configuracao no Node-RED

Ao criar o servidor MQTT no Node-RED:

- **Server:** `broker.hivemq.com`
- **Port:** `8883`
- **Use TLS:** habilitado
- **Protocol Version:** `4`
- **Client ID:** pode deixar em branco ou definir um nome unico

Se estiver usando o FlowFuse Dashboard, primeiro instale:

```bash
npm install @flowfuse/node-red-dashboard
```

## Ajuste para atualizacao rapida

Para deixar a atividade mais responsiva sem sobrecarregar tanto o broker publico:

- leitura de sensores a cada `500 ms`;
- publicacao de telemetria a cada `500 ms`;
- publicacao de status a cada `250 ms`;
- `keep alive` MQTT em `15 s`;
- reconexao de Wi-Fi e MQTT a cada `1500 ms`.

Observacao:

- o `broker.hivemq.com` e publico e compartilhado;
- taxas muito agressivas podem aumentar quedas, throttling ou reconexoes;
- para demonstracao em aula, `500 ms` costuma ser um ponto de equilibrio melhor que publicar em loop continuo.

## Padrao de topicos

Substitua `gX` pelo numero do seu grupo. Exemplo: `g7`.

### Telemetria do ESP32 para o Node-RED

- `satc/gX/telemetry/temperature`
- `satc/gX/telemetry/humidity`
- `satc/gX/telemetry/luminosity`

### Comando do Node-RED para o ESP32

- `satc/gX/cmd/light`

### Status do ESP32 para o Node-RED

- `satc/gX/status/focus`
- `satc/gX/status/alert`

## Exemplo para o grupo 7

```text
satc/g7/telemetry/temperature
satc/g7/telemetry/humidity
satc/g7/telemetry/luminosity
satc/g7/cmd/light
satc/g7/status/focus
satc/g7/status/alert
```

## Payloads esperados

### Telemetria

```text
satc/g7/telemetry/temperature -> 24.5
satc/g7/telemetry/humidity -> 55.0
satc/g7/telemetry/luminosity -> 37
```

### Comando

```text
satc/g7/cmd/light -> ON
satc/g7/cmd/light -> OFF
```

### Status

```text
satc/g7/status/focus -> IDLE
satc/g7/status/focus -> FOCUS
satc/g7/status/focus -> PAUSE

satc/g7/status/alert -> OK
satc/g7/status/alert -> WARN
satc/g7/status/alert -> ALERT
```

## Como testar rapidamente

### Publicar comando manual

No Node-RED ou em um cliente MQTT, publique:

- **Topico:** `satc/g7/cmd/light`
- **Payload:** `ON`

Depois teste:

- **Topico:** `satc/g7/cmd/light`
- **Payload:** `OFF`

### Assinar telemetria

Escute estes topicos:

- `satc/g7/telemetry/temperature`
- `satc/g7/telemetry/humidity`
- `satc/g7/telemetry/luminosity`

## Resumo

```text
Broker: broker.hivemq.com
Porta: 8883
Grupo: g7
Comando principal: satc/g7/cmd/light
Telemetria: satc/g7/telemetry/*
Status: satc/g7/status/*
```
