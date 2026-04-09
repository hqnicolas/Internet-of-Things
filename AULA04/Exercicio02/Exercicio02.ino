#include <WiFi.h>
#include <WebServer.h>

const char *SSID_AP = "ESP32_IoT_Aula04";
const char *PASSWORD_AP = "12345678";

const uint8_t LED_PINS[] = {4, 0, 2, 15};
bool ledStates[] = {false, false, false, false};

WebServer server(80);

void applyLedState(uint8_t index, bool state) {
  ledStates[index] = state;
  digitalWrite(LED_PINS[index], state ? HIGH : LOW);
}

String buildPage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Controle de LEDs</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #f4f7fb;
      color: #1f2937;
      margin: 0;
      padding: 24px;
    }
    main {
      max-width: 720px;
      margin: 0 auto;
      background: #ffffff;
      border-radius: 18px;
      padding: 24px;
      box-shadow: 0 12px 32px rgba(15, 23, 42, 0.08);
    }
    .led {
      display: grid;
      grid-template-columns: 1fr auto auto auto;
      gap: 12px;
      align-items: center;
      padding: 12px 0;
      border-bottom: 1px solid #e5e7eb;
    }
    .status-on {
      color: #15803d;
      font-weight: bold;
    }
    .status-off {
      color: #b91c1c;
      font-weight: bold;
    }
    button {
      border: 0;
      border-radius: 10px;
      padding: 10px 14px;
      cursor: pointer;
      font-weight: bold;
    }
    .on {
      background: #16a34a;
      color: white;
    }
    .off {
      background: #dc2626;
      color: white;
    }
    .all {
      background: #0f766e;
      color: white;
      margin-top: 20px;
      margin-right: 8px;
    }
  </style>
</head>
<body>
  <main>
    <h1>Controle de LEDs da Shield</h1>
    <p>Use os botoes abaixo para ligar e desligar os LEDs conectados ao ESP32.</p>
  )rawliteral";

  for (uint8_t i = 0; i < 4; i++) {
    html += "<section class=\"led\">";
    html += "<strong>LED " + String(i + 1) + "</strong>";
    html += "<span class=\"" + String(ledStates[i] ? "status-on" : "status-off") + "\">";
    html += ledStates[i] ? "Ligado" : "Desligado";
    html += "</span>";
    html += "<button class=\"on\" onclick=\"sendCommand(" + String(i) + ",1)\">ON</button>";
    html += "<button class=\"off\" onclick=\"sendCommand(" + String(i) + ",0)\">OFF</button>";
    html += "</section>";
  }

  html += R"rawliteral(
    <button class="all" onclick="sendAll(1)">Ligar todos</button>
    <button class="all" onclick="sendAll(0)">Desligar todos</button>
  </main>
  <script>
    function sendCommand(led, state) {
      fetch(`/led?num=${led}&state=${state}`, { method: 'POST' })
        .then(() => window.location.reload());
    }

    function sendAll(state) {
      fetch(`/all?state=${state}`, { method: 'POST' })
        .then(() => window.location.reload());
    }
  </script>
</body>
</html>
  )rawliteral";

  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=utf-8", buildPage());
}

void handleLed() {
  if (!server.hasArg("num") || !server.hasArg("state")) {
    server.send(400, "text/plain", "Parametros num e state sao obrigatorios.");
    return;
  }

  int ledIndex = server.arg("num").toInt();
  int state = server.arg("state").toInt();

  if (ledIndex < 0 || ledIndex >= 4) {
    server.send(400, "text/plain", "LED invalido.");
    return;
  }

  applyLedState(static_cast<uint8_t>(ledIndex), state == 1);
  server.send(200, "application/json", "{\"ok\":true}");
}

void handleAll() {
  if (!server.hasArg("state")) {
    server.send(400, "text/plain", "Parametro state e obrigatorio.");
    return;
  }

  bool state = server.arg("state").toInt() == 1;
  for (uint8_t i = 0; i < 4; i++) {
    applyLedState(i, state);
  }

  server.send(200, "application/json", "{\"ok\":true}");
}

void setup() {
  Serial.begin(115200);
  delay(200);

  for (uint8_t i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    applyLedState(i, false);
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID_AP, PASSWORD_AP);

  Serial.println();
  Serial.println("Exercicio 02 - WebServer de LEDs");
  Serial.print("Rede Wi-Fi criada: ");
  Serial.println(SSID_AP);
  Serial.print("IP do Access Point: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/led", HTTP_POST, handleLed);
  server.on("/all", HTTP_POST, handleAll);
  server.begin();

  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  server.handleClient();
}
