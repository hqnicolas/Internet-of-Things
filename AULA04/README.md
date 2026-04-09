# ROTEIRO PRÁTICO - WebServer com ESP32
**Disciplina:** Internet das Coisas | **Aula:** 09/04/2026  

---

## INFORMAÇÕES GERAIS
Esta aula prática é dividida em 4 atividades progressivas. Cada atividade constrói sobre a anterior, começando com um servidor web básico e evoluindo para um servidor assíncrono com integração completa de sensores. 

---

## EXEMPLO 1: WebServer Básico com Access Point
**Objetivo:** Criar um servidor web simples no ESP32 como Access Point (AP). O ESP32 criará sua própria rede WiFi e responderá com uma página HTML básica.

### DICAS IMPORTANTES:
- Use apenas a biblioteca `<WebServer.h>` nesta atividade — sem bibliotecas extras  
- Modo AP significa "Access Point": o ESP32 cria uma rede, você conecta a ele  
- A página HTML deve estar dentro de aspas duplas no código C++  
- Documento de 4-5 conexões simultâneas: aumentar `HTTPD_MAX_URI_HANDLERS` para 20 se necessário  

### CÓDIGO MODELO:
```cpp
#include <WebServer.h>
#include <WiFi.h>

#define SSID_AP "ESP32_IoT_Grupo_X"
#define PASSWORD_AP "12345678"

WebServer server(80);

void handleRoot() {
  String html = R"(
    <html>
      <head><title>ESP32 Server</title></head>
      <body>
        <h1>Bem-vindo!</h1>
        <p>Servidor web no ESP32</p>
      </body>
    </html>
  )";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID_AP, PASSWORD_AP);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP: ");
  Serial.println(IP);
  
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
````

### PASSOS:

1. **Copie o código acima**

   * Cole no Arduino IDE. Certifique-se que a placa é "ESP32 Dev Module" e porta está correta.

2. **Carregue no ESP32**

   * Clique em "Upload". Aguarde a mensagem "Done uploading".

3. **Abra o Serial Monitor**

   * Configure para 115200 baud. Procure pela mensagem "AP IP: 192.168.4.x".

4. **Conecte à rede WiFi**

   * No seu smartphone/PC, procure por "ESP32_IoT" (senha: 12345678).

5. **Acesse a página**

   * Abra navegador e digite: [http://192.168.4.1](http://192.168.4.1) (a porta 80 é padrão).

6. **Verifique a resposta**

   * Você deve ver a página com "Bem-vindo!" - sucesso!

### ✓ CHECKLIST DE CONCLUSÃO:

* ☐ Código compilado sem erros
* ☐ Serial Monitor mostra IP do AP
* ☐ WiFi "ESP32_IoT_Grupo_X" visível
* ☐ Navegador carrega página HTML
* ☐ Anotou o IP (ex: 192.168.4.1) para próximas atividades

---

## ATIVIDADE 1: Controle de LEDs via Web

**Objetivo:** Integrar os LEDs da shield no servidor web. A página exibirá botões para ligar/desligar cada LED. As mudanças serão visíveis em tempo real na shield.

### DICAS IMPORTANTES:

* Pinos dos LEDs: GPIO 4, 0, 2, 15 (confira no código exemplo do repositório)
* Use `<button>` tags com `onclick` para enviar requisições POST
* Cada clique envia uma requisição que a função `handleLED` processa
* Configure `pinMode(LED_PIN, OUTPUT)` no setup()
* Teste a página em seu computador (abra um arquivo HTML local) **ANTES** de carregar no ESP32 para garantir o funcionamento
* Use `digitalWrite(LED_PIN, HIGH/LOW)` para controlar os pinos

### EXEMPLO DE ESTRUTURA DO CÓDIGO (NÃO FUNCIONAL):

```cpp
// … Código anterior … 

#define LED1 4
#define LED2 0
#define LED3 2
#define LED4 15

bool ledState[4] = {false, false, false, false};

void handleLED() {
  if (server.hasArg("led") && server.hasArg("state")) {
    int ledNum = server.arg("led").toInt();
    int state = server.arg("state").toInt();
    
    if (ledNum >= 0 && ledNum < 4) {
      digitalWrite(leds[ledNum], state ? HIGH : LOW);
      ledState[ledNum] = state;
    }
  }
  server.send(200, "application/json", "{\"ok\":true}");
}

void setup() {
  // ... código anterior ...
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  
  server.on("/led", HTTP_POST, handleLED);
  server.on("/", handleRoot);
  server.begin();
}
```

### 🌐 EXEMPLO DE HTML:

```html
<html>
<head><title>Controle LEDs</title></head>
<body>
  <h1>Controle de LEDs</h1>
  <button onclick="toggleLED(0, true)">LED 1 ON</button>
  <button onclick="toggleLED(0, false)">LED 1 OFF</button>
  <br><br>
  <button onclick="toggleLED(1, true)">LED 2 ON</button>
  <button onclick="toggleLED(1, false)">LED 2 OFF</button>
  
  <script>
    function toggleLED(num, state) {
      fetch('/led?led=' + num + '&state=' + (state ? 1 : 0), 
        {method: 'POST'});
    }
  </script>
</body>
</html>
```

---

## ATIVIDADE 2: Cliente WiFi + Leitura de Sensores

**Objetivo:** Conectar o ESP32 à rede WiFi (cliente). Ler sensores e exibir valores em uma página web.

### 💡 DICAS IMPORTANTES:

* DHT11: GPIO 33 — use `<DHT.h>`
* LDR: GPIO 39 — `analogRead(39)` → 0-100%
* Potenciômetro: GPIO 34 — `analogRead(34)`
* Use `WIFI_STA` em vez de `WIFI_AP`
* Use `WiFi.begin(ssid, password)`

### 📝 EXEMPLO DE CÓDIGO (NÃO FUNCIONAL):

```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHT_PIN 33
#define LDR_PIN 39
#define POT_PIN 34

#define SSID "seu_wifi"
#define PASSWORD "sua_senha"

DHT dht(DHT_PIN, DHT11);
WebServer server(80);

void handleSensors() {
  float temp = dht.readTemperature();
  float umidade = dht.readHumidity();
  int luz = map(analogRead(LDR_PIN), 0, 4095, 0, 100);
  int pot = analogRead(POT_PIN);
  
  StaticJsonDocument<200> doc;
  doc["temp"] = temp;
  doc["umidade"] = umidade;
  doc["luz"] = luz;
  doc["pot"] = pot;
  
  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}
```

### PASSOS:

1. Mude para modo cliente
2. Configure credenciais WiFi
3. Inclua bibliotecas
4. Implemente `handleSensors()`
5. Exiba valores na página
6. Teste no navegador

### ✓ CHECKLIST:

* ☐ ESP32 conectado à WiFi
* ☐ Temperatura/umidade variam
* ☐ Luz responde ao ambiente
* ☐ Potenciômetro varia valores
* ☐ Página atualiza a cada 1s

---

## REFERÊNCIA RÁPIDA: Pinos da Shield

| Componente    | GPIO   | Tipo        |
| ------------- | ------ | ----------- |
| LED 1         | 4      | Digital OUT |
| LED 2         | 0      | Digital OUT |
| LED 3         | 2      | Digital OUT |
| LED 4         | 15     | Digital OUT |
| DHT11         | 33     | Digital IN  |
| LDR           | 39     | Analog IN   |
| Potenciômetro | 34     | Analog IN   |
| Relé          | 13     | Digital OUT |
| Display 7-Seg | 16, 17 | Digital OUT |

---

## DÚVIDAS COMUNS

**1. Como determinar o IP do ESP32?**

* Serial Monitor mostra ao conectar

**2. Erro: library not found**

* Instale via Library Manager

**3. LEDs não acendem**

* Verifique pino, `pinMode` e `digitalWrite`

**4. Página não carrega**

* Verifique IP e Serial Monitor

**5. Sensores retornam valores errados**

* Use `map()` e respeite tempo do DHT11

**6. Servidor bloqueia**

* Normal — será resolvido com AsyncWebServer
