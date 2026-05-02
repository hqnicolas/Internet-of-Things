# Aula 06 - Atividade CoAP com ESP32, LDR e LED

## Arquivos da Pasta

- `Atividade.ino`: sketch principal da atividade
- `README.md`: guia rapido de configuracao, upload e testes

---

## Objetivo

Implementar um servidor **CoAP** no **ESP32** usando o mesmo hardware das aulas anteriores, com:

- leitura do **LDR** no `GPIO 39`
- controle de **LED** por `toggle` no `GPIO 2`
- resposta em tres recursos CoAP: `/status`, `/ldr` e `/led`

---

## Bibliotecas Necessarias

Instale na Arduino IDE:

- `WiFi` para ESP32
- `WiFiUdp`
- `CoAP-simple-library`

Biblioteca CoAP usada no sketch:

- repositorio: `hirotakaster/CoAP-simple-library`
- include esperado no codigo: `#include <coap-simple.h>`

---

## Hardware Utilizado

| Componente | GPIO | Tipo |
| :--- | :--- | :--- |
| LDR | 39 | Entrada analogica |
| LED | 2 | Saida digital |

---

## Recursos CoAP Implementados

### `GET /status`

Resposta em texto simples:

```text
ESP32 CoAP online
```

### `GET /ldr`

Resposta em JSON com leitura bruta e percentual de luminosidade:

```json
{"raw":2780,"luz":67}
```

### `GET /led`

Alterna o estado do LED a cada requisicao e responde em JSON:

```json
{"led":"on"}
```

Na chamada seguinte, a resposta esperada sera:

```json
{"led":"off"}
```

---

## Configuracao do Codigo

Abra `Atividade.ino` e altere estas linhas:

```cpp
const char *WIFI_SSID = "SEU_WIFI";
const char *WIFI_PASSWORD = "SUA_SENHA";
```

Depois confirme:

- placa: `ESP32 Dev Module`
- porta serial correta
- baud rate do monitor serial: `115200`

---

## Como Gravar no ESP32

1. Abra `AULA06/Atividade/Atividade.ino` na Arduino IDE.
2. Instale as bibliotecas necessarias.
3. Configure SSID e senha.
4. Selecione a placa `ESP32 Dev Module`.
5. Envie o codigo para a placa.
6. Abra o monitor serial em `115200`.

No monitor serial, voce deve ver:

- tentativa de conexao com o Wi-Fi
- IP local do ESP32
- confirmacao de inicio do servidor CoAP
- lista dos recursos disponiveis

---

## Testes com `coap-client`

Substitua `IP_DO_ESP32` pelo IP mostrado no monitor serial.

Verificar status:

```bash
coap-client -m get coap://IP_DO_ESP32/status
```

Ler luminosidade:

```bash
coap-client -m get coap://IP_DO_ESP32/ldr
```

Alternar LED:

```bash
coap-client -m get coap://IP_DO_ESP32/led
```

Alternar novamente:

```bash
coap-client -m get coap://IP_DO_ESP32/led
```

---

## Comportamento Esperado

- o ESP32 conecta na rede em modo `WIFI_STA`
- o recurso `/status` confirma que o dispositivo esta online
- o recurso `/ldr` retorna JSON valido com `raw` e `luz`
- o valor de `luz` muda conforme a iluminacao ambiente
- o recurso `/led` alterna o LED a cada chamada
- se o Wi-Fi cair, o `loop()` tenta reconectar automaticamente

---

## Checklist

* ☐ ESP32 conectou no Wi-Fi
* ☐ IP foi exibido no monitor serial
* ☐ `/status` respondeu corretamente
* ☐ `/ldr` retornou JSON valido
* ☐ a leitura do LDR variou com a luz ambiente
* ☐ `/led` ligou o LED na primeira chamada
* ☐ `/led` desligou o LED na segunda chamada

---

## Desafio Extra

- adicionar um recurso `/led/on` e outro `/led/off`
- responder `/ldr` tambem em texto simples
- classificar o ambiente como `claro` ou `escuro`
