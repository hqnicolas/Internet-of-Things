# Projeto: Smart Desk Ergonomica IoT

**Foco:** produtividade, saude ocupacional e eficiencia energetica.

Este projeto transforma uma mesa de trabalho comum em um sistema inteligente que monitora o ambiente, gerencia ciclos de concentracao e automatiza a iluminacao. Para a comunicacao MQTT, sera utilizado o broker publico `broker.hivemq.com`, seguindo o mesmo padrao adotado em [AULA07/Atividade/Atividade.ino](/c:/Users/nicolas.337911/Downloads/Internet-of-Things/AULA07/Atividade/Atividade.ino).

---

## 1. Arquitetura do hardware

O **ESP32** atua como dispositivo de borda, lendo sensores, processando regras locais e trocando mensagens com o broker MQTT.

### Entradas

* **DHT11:** leitura de temperatura e umidade relativa do ar.
* **LDR (analogico):** medicao da luminosidade sobre a mesa.
* **4 botoes digitais:**
1. **Start/Pause:** inicia ou pausa o cronometro.
2. **Reset:** reinicia o ciclo atual.
3. **Toggle Display:** alterna a informacao mostrada nos displays.
4. **Manual Override:** liga ou desliga a iluminacao manualmente.

### Saidas

* **2 displays de 7 segmentos:** exibicao de valores de `00` a `99`.
* **1 rele:** acionamento da luminaria.
* **4 LEDs de status:**
* **LED 1:** modo foco ativo.
* **LED 2:** pausa em andamento.
* **LED 3:** conexao Wi-Fi/MQTT ativa.
* **LED 4:** alerta ambiental.

---

## 2. Conectividade de rede

O ESP32 deve se conectar ao Wi-Fi institucional **`SATC 2.4`** usando **WPA2 Enterprise**, como no exemplo da aula 07.

### Bibliotecas utilizadas

* `WiFi.h`
* `WiFiClientSecure.h`
* `PubSubClient.h`

### Credenciais de exemplo

```cpp
const char *WIFI_SSID = "SATC 2.4";
const char *EAP_IDENTITY = "aluno.123456@alunosatc.edu.br";
const char *EAP_USERNAME = "aluno.123456@alunosatc.edu.br";
const char *EAP_PASSWORD = "123456";
```

### Inicializacao da conexao

```cpp
WiFi.disconnect(true);
WiFi.mode(WIFI_STA);
WiFi.begin(WIFI_SSID, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
```

---

## 3. Comunicacao MQTT

O projeto utilizara o broker publico **`broker.hivemq.com`** com cliente seguro baseado em `WiFiClientSecure`.

### Configuracao base

```cpp
const char *MQTT_HOST = "broker.hivemq.com";
constexpr int MQTT_PORT = 8883;

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);
```

### Observacoes importantes

* A porta utilizada sera **`8883`**, padrao para MQTT seguro.
* O cliente seguro sera criado com **`WiFiClientSecure.h`**.
* No exemplo atual da aula 07, o certificado nao e validado e o codigo usa:

```cpp
espClient.setInsecure();
```

Isso simplifica os testes em laboratorio, mas nao e o modelo ideal para producao.

### Estrategia de topicos

Seguindo o padrao da aula 07, os topicos podem ser organizados por grupo:

* `satc/g7/led1`
* `satc/g7/led2`
* `satc/g7/led3`
* `satc/g7/led4`
* `satc/g7/all`
* `satc/all/all`

Esse modelo permite:

* controlar dispositivos individualmente;
* enviar comandos para todo o grupo;
* compartilhar comandos globais entre todos os grupos.

---

## 4. Estrategia da atividade

Para cumprir os requisitos da disciplina, o projeto pode usar:

1. **MQTT (obrigatorio):**
* publicacao de dados de temperatura, umidade e luminosidade;
* assinatura de topicos para comandos remotos, como acionamento da iluminacao ou troca de estado dos LEDs.

2. **HTTP WebServer (alternativa complementar):**
* hospedagem de uma pagina simples no ESP32;
* exibicao de status local;
* configuracao de parametros, como tempo de foco e limites de luminosidade.

---

## 5. Node-RED e automacao

O **Node-RED** funcionara como camada de supervisao e automacao.

### Possibilidades no dashboard

* gauges para temperatura e umidade;
* grafico historico da luminosidade;
* botoes para envio de comandos MQTT;
* indicadores de conexao e status do ciclo.

### Regras de exemplo

1. Se a luminosidade estiver abaixo de um limite e o modo foco estiver ativo, ligar a luminaria.
2. Se a umidade ficar abaixo de `30%`, gerar alerta visual.
3. Registrar eventos e leituras para analise posterior.

---

## 6. Diferenciais tecnicos

* reconexao automatica de Wi-Fi e MQTT;
* uso de `WiFiClientSecure` para conexao com broker na porta `8883`;
* organizacao de topicos por grupo;
* possibilidade de operacao local mesmo durante instabilidades na rede.
