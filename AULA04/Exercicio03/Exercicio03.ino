#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

const char *SSID_AP = "ESP32_IoT_Aula04";
const char *PASSWORD_AP = "12345678";

const uint8_t DHT_PIN = 33;
const uint8_t LDR_PIN = 39;
const uint8_t POT_PIN = 34;
const uint8_t DHT_TYPE = DHT11;

DHT dht(DHT_PIN, DHT_TYPE);
WebServer server(80);

struct SensorData {
  float temperature;
  float humidity;
  int ldrRaw;
  int ldrPercent;
  int potentiometer;
  bool dhtOk;
};

SensorData readSensors() {
  SensorData data;
  data.temperature = dht.readTemperature();
  data.humidity = dht.readHumidity();
  data.ldrRaw = analogRead(LDR_PIN);
  data.ldrPercent = map(data.ldrRaw, 0, 4095, 0, 100);
  data.potentiometer = analogRead(POT_PIN);
  data.dhtOk = !isnan(data.temperature) && !isnan(data.humidity);
  return data;
}

String buildPage(const SensorData &data) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Monitor de Sensores</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #eef2ff;
      color: #1f2937;
      margin: 0;
      padding: 24px;
    }
    main {
      max-width: 760px;
      margin: 0 auto;
      background: white;
      border-radius: 20px;
      padding: 28px;
      box-shadow: 0 16px 40px rgba(30, 41, 59, 0.12);
    }
    .grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
      gap: 16px;
      margin-top: 20px;
    }
    .card {
      background: #f8fafc;
      border: 1px solid #e2e8f0;
      border-radius: 16px;
      padding: 18px;
    }
    .label {
      font-size: 0.9rem;
      color: #475569;
      margin-bottom: 8px;
    }
    .value {
      font-size: 1.8rem;
      font-weight: bold;
      color: #0f172a;
    }
    .warning {
      color: #b91c1c;
      font-weight: bold;
    }
  </style>
</head>
<body>
  <main>
    <h1>Valores dos Sensores da Shield</h1>
    <p>Atualizacao automatica a cada 2 segundos usando um webserver sincronico.</p>
    <div class="grid">
  )rawliteral";

  html += "<section class=\"card\"><div class=\"label\">Temperatura</div><div class=\"value\">";
  html += data.dhtOk ? String(data.temperature, 1) + " C" : "--";
  html += "</div></section>";

  html += "<section class=\"card\"><div class=\"label\">Umidade</div><div class=\"value\">";
  html += data.dhtOk ? String(data.humidity, 1) + " %" : "--";
  html += "</div></section>";

  html += "<section class=\"card\"><div class=\"label\">Luminosidade</div><div class=\"value\">";
  html += String(data.ldrPercent) + " %";
  html += "</div></section>";

  html += "<section class=\"card\"><div class=\"label\">Potenciometro</div><div class=\"value\">";
  html += String(data.potentiometer);
  html += "</div></section>";

  html += "</div>";

  if (!data.dhtOk) {
    html += "<p class=\"warning\">Falha ao ler o DHT11. Verifique o sensor e tente novamente.</p>";
  }

  html += R"rawliteral(
    <p><small>Os dados desta pagina sao atualizados automaticamente.</small></p>
  </main>
  <script>
    setTimeout(() => window.location.reload(), 2000);
  </script>
</body>
</html>
  )rawliteral";

  return html;
}

String buildJson(const SensorData &data) {
  String json = "{";
  json += "\"temperatura\":";
  json += data.dhtOk ? String(data.temperature, 1) : "null";
  json += ",\"umidade\":";
  json += data.dhtOk ? String(data.humidity, 1) : "null";
  json += ",\"luminosidade_percentual\":";
  json += String(data.ldrPercent);
  json += ",\"luminosidade_bruta\":";
  json += String(data.ldrRaw);
  json += ",\"potenciometro\":";
  json += String(data.potentiometer);
  json += "}";
  return json;
}

void handleRoot() {
  SensorData data = readSensors();
  server.send(200, "text/html; charset=utf-8", buildPage(data));
}

void handleSensors() {
  SensorData data = readSensors();
  server.send(200, "application/json", buildJson(data));
}

void handleNotFound() {
  server.send(404, "text/plain", "Rota nao encontrada.");
}

void startAccessPoint() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID_AP, PASSWORD_AP);

  Serial.print("Rede Wi-Fi criada: ");
  Serial.println(SSID_AP);
  Serial.print("IP do Access Point: ");
  Serial.println(WiFi.softAPIP());
}

void setup() {
  Serial.begin(115200);
  delay(200);

  dht.begin();
  analogReadResolution(12);

  Serial.println();
  Serial.println("Exercicio 03 - WebServer de sensores");

  startAccessPoint();

  server.on("/", HTTP_GET, handleRoot);
  server.on("/sensores", HTTP_GET, handleSensors);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  server.handleClient();
}
