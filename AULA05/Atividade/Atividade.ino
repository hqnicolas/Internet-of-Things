#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

static const char *WIFI_SSID = "SEU_WIFI";
static const char *WIFI_PASSWORD = "SUA_SENHA";
static const char *AP_SSID = "ESP32_IoT_Aula05";
static const char *AP_PASSWORD = "12345678";

static const uint8_t DHT_PIN = 33;
static const uint8_t DHT_TYPE = DHT11;
static const uint8_t LDR_PIN = 39;

static const uint8_t LED_RED = 4;
static const uint8_t LED_GREEN = 0;
static const uint8_t LED_BLUE = 2;

static const uint8_t RGB_RED = 25;
static const uint8_t RGB_GREEN = 26;
static const uint8_t RGB_BLUE = 27;

static const uint8_t SEG_A = 18;
static const uint8_t SEG_B = 5;
static const uint8_t SEG_C = 21;
// static const uint8_t SEG_D = 3;
// static const uint8_t SEG_E = 1;
static const uint8_t SEG_F = 23;
static const uint8_t SEG_G = 22;
static const uint8_t SEG_DP = 19;
static const uint8_t DISPLAY_DEZENA = 16;
static const uint8_t DISPLAY_UNIDADE = 17;

static const bool SEGMENTS_ACTIVE_HIGH = true;
static const bool DIGITS_ACTIVE_HIGH = true;

static const uint16_t PWM_FREQUENCY = 5000;
static const uint8_t PWM_RESOLUTION = 8;

struct SensorData {
  float temperature = NAN;
  float humidity = NAN;
  int ldrRaw = 0;
  int ldrPercent = 0;
  bool dhtOk = false;
};

DHT dht(DHT_PIN, DHT_TYPE);
AsyncWebServer server(80);

bool ledStates[3] = {false, false, false};
uint8_t rgbValues[3] = {0, 0, 0};

SensorData currentData;
unsigned long lastSensorReadMs = 0;
unsigned long lastDisplayMuxMs = 0;
uint8_t activeDisplayIndex = 0;
int displayValue = 0;

const uint8_t SEGMENT_PINS[8] = {
  // SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G, SEG_DP
  SEG_A, SEG_B, SEG_C, SEG_F, SEG_G, SEG_DP
};

const uint8_t DIGIT_PINS[2] = {
  DISPLAY_DEZENA, DISPLAY_UNIDADE
};

const uint8_t LED_PINS[3] = {
  LED_RED, LED_GREEN, LED_BLUE
};

const bool DIGIT_MAP[10][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1, 0},
  {1, 1, 1, 1, 0, 0, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {1, 0, 1, 1, 0, 1, 1, 0},
  {1, 0, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 0, 1, 1, 0}
};

void writeSegment(uint8_t pin, bool enabled) {
  digitalWrite(pin, enabled == SEGMENTS_ACTIVE_HIGH ? HIGH : LOW);
}

void setDigitEnabled(uint8_t pin, bool enabled) {
  digitalWrite(pin, enabled == DIGITS_ACTIVE_HIGH ? HIGH : LOW);
}

void disableAllDigits() {
  for (uint8_t i = 0; i < 2; i++) {
    setDigitEnabled(DIGIT_PINS[i], false);
  }
}

void showDigit(uint8_t digit) {
  if (digit > 9) {
    digit = 0;
  }

  for (uint8_t i = 0; i < 8; i++) {
    writeSegment(SEGMENT_PINS[i], DIGIT_MAP[digit][i]);
  }
}

void refreshDisplay() {
  unsigned long now = millis();
  if (now - lastDisplayMuxMs < 4) {
    return;
  }
  lastDisplayMuxMs = now;

  int safeValue = constrain(displayValue, 0, 99);
  uint8_t dezenas = safeValue / 10;
  uint8_t unidades = safeValue % 10;
  uint8_t digitToShow = activeDisplayIndex == 0 ? dezenas : unidades;

  disableAllDigits();
  showDigit(digitToShow);
  setDigitEnabled(DIGIT_PINS[activeDisplayIndex], true);

  activeDisplayIndex = (activeDisplayIndex + 1) % 2;
}

void setDiscreteLed(uint8_t index, bool state) {
  if (index >= 3) {
    return;
  }

  ledStates[index] = state;
  digitalWrite(LED_PINS[index], state ? HIGH : LOW);
}

void applyRgb() {
  ledcWrite(RGB_RED, rgbValues[0]);
  ledcWrite(RGB_GREEN, rgbValues[1]);
  ledcWrite(RGB_BLUE, rgbValues[2]);
}

SensorData readSensors() {
  SensorData data;
  data.temperature = dht.readTemperature();
  data.humidity = dht.readHumidity();
  data.ldrRaw = analogRead(LDR_PIN);
  data.ldrPercent = map(data.ldrRaw, 0, 4095, 0, 100);
  data.ldrPercent = constrain(data.ldrPercent, 0, 100);
  data.dhtOk = !isnan(data.temperature) && !isnan(data.humidity);
  return data;
}

void updateSensorData() {
  unsigned long now = millis();
  if (now - lastSensorReadMs < 2000) {
    return;
  }

  lastSensorReadMs = now;
  currentData = readSensors();

  if (currentData.dhtOk) {
    int roundedTemperature = static_cast<int>(currentData.temperature + 0.5f);
    displayValue = constrain(roundedTemperature, 0, 99);
  } else {
    displayValue = 0;
  }

  Serial.print("Sensores -> Temp: ");
  if (currentData.dhtOk) {
    Serial.print(currentData.temperature, 1);
    Serial.print(" C | Umidade: ");
    Serial.print(currentData.humidity, 1);
    Serial.print(" % | LDR: ");
    Serial.print(currentData.ldrPercent);
    Serial.println(" %");
  } else {
    Serial.print("falha no DHT | LDR: ");
    Serial.print(currentData.ldrPercent);
    Serial.println(" %");
  }
}

String buildJson() {
  String json = "{";
  json += "\"temperatura\":";
  json += currentData.dhtOk ? String(currentData.temperature, 1) : "null";
  json += ",\"umidade\":";
  json += currentData.dhtOk ? String(currentData.humidity, 1) : "null";
  json += ",\"luminosidade_percentual\":";
  json += String(currentData.ldrPercent);
  json += ",\"luminosidade_bruta\":";
  json += String(currentData.ldrRaw);
  json += ",\"leds\":{";
  json += "\"vermelho\":";
  json += ledStates[0] ? "true" : "false";
  json += ",\"verde\":";
  json += ledStates[1] ? "true" : "false";
  json += ",\"azul\":";
  json += ledStates[2] ? "true" : "false";
  json += "},\"rgb\":{";
  json += "\"r\":";
  json += String(rgbValues[0]);
  json += ",\"g\":";
  json += String(rgbValues[1]);
  json += ",\"b\":";
  json += String(rgbValues[2]);
  json += "},\"display\":";
  json += String(displayValue);
  json += "}";
  return json;
}

String buildPage() {
  return R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>IoT Smart Control Center</title>
  <style>
    :root {
      --bg-1: #f7f4ea;
      --bg-2: #d6e4d9;
      --panel: rgba(255, 252, 245, 0.88);
      --text: #1c2a25;
      --muted: #55635d;
      --border: rgba(28, 42, 37, 0.12);
      --shadow: 0 24px 60px rgba(28, 42, 37, 0.14);
      --accent: #1f7a63;
      --warm: #df7b2d;
      --danger: #c84545;
    }

    * {
      box-sizing: border-box;
    }

    body {
      margin: 0;
      min-height: 100vh;
      font-family: Verdana, Geneva, sans-serif;
      color: var(--text);
      background:
        radial-gradient(circle at top left, rgba(223, 123, 45, 0.25), transparent 32%),
        radial-gradient(circle at bottom right, rgba(31, 122, 99, 0.28), transparent 28%),
        linear-gradient(160deg, var(--bg-1), var(--bg-2));
      padding: 20px;
    }

    .app {
      width: min(1100px, 100%);
      margin: 0 auto;
      background: var(--panel);
      backdrop-filter: blur(10px);
      border: 1px solid var(--border);
      border-radius: 28px;
      box-shadow: var(--shadow);
      padding: 24px;
    }

    .hero {
      display: grid;
      gap: 16px;
      margin-bottom: 20px;
    }

    h1 {
      margin: 0;
      font-size: clamp(2rem, 4vw, 3.4rem);
      line-height: 0.95;
      letter-spacing: -0.04em;
    }

    .subtitle {
      margin: 0;
      color: var(--muted);
      max-width: 640px;
    }

    .grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(230px, 1fr));
      gap: 16px;
    }

    .card {
      background: rgba(255, 255, 255, 0.78);
      border: 1px solid var(--border);
      border-radius: 22px;
      padding: 18px;
    }

    .label {
      font-size: 0.88rem;
      text-transform: uppercase;
      letter-spacing: 0.08em;
      color: var(--muted);
      margin-bottom: 8px;
    }

    .value {
      font-size: clamp(1.8rem, 4vw, 2.6rem);
      font-weight: 700;
    }

    .pill {
      display: inline-flex;
      align-items: center;
      gap: 8px;
      background: rgba(31, 122, 99, 0.12);
      color: var(--accent);
      border-radius: 999px;
      padding: 8px 14px;
      font-size: 0.92rem;
    }

    .controls {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
      gap: 16px;
      margin-top: 16px;
    }

    .button-row {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 10px;
    }

    button {
      border: 0;
      border-radius: 16px;
      padding: 14px 12px;
      font-weight: 700;
      cursor: pointer;
      color: white;
      transition: transform 0.15s ease, opacity 0.15s ease;
    }

    button:hover {
      transform: translateY(-2px);
      opacity: 0.94;
    }

    .btn-red { background: #d94a46; }
    .btn-green { background: #1f8f62; }
    .btn-blue { background: #2563c9; }

    .status-list {
      display: grid;
      gap: 10px;
      margin-top: 14px;
    }

    .status-item {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 12px 14px;
      background: rgba(255, 255, 255, 0.75);
      border-radius: 14px;
      border: 1px solid var(--border);
    }

    .badge {
      padding: 6px 10px;
      border-radius: 999px;
      font-size: 0.82rem;
      font-weight: 700;
    }

    .badge-on {
      background: rgba(31, 143, 98, 0.16);
      color: #17684b;
    }

    .badge-off {
      background: rgba(200, 69, 69, 0.14);
      color: #8c2f2f;
    }

    .slider-wrap {
      display: grid;
      gap: 14px;
      margin-top: 12px;
    }

    .slider-group {
      display: grid;
      gap: 8px;
    }

    input[type="range"] {
      width: 100%;
      accent-color: var(--accent);
    }

    .color-preview {
      height: 84px;
      border-radius: 18px;
      border: 1px solid var(--border);
      background: rgb(0, 0, 0);
      margin-top: 14px;
      box-shadow: inset 0 0 0 1px rgba(255, 255, 255, 0.35);
    }

    .footer-note {
      margin-top: 18px;
      color: var(--muted);
      font-size: 0.92rem;
    }

    @media (max-width: 640px) {
      .app {
        padding: 18px;
        border-radius: 20px;
      }

      .button-row {
        grid-template-columns: 1fr;
      }
    }
  </style>
</head>
<body>
  <main class="app">
    <section class="hero">
      <div class="pill">ESP32 + AsyncWebServer + Shield IoT</div>
      <h1>Smart Control Center</h1>
      <p class="subtitle">
        Monitoramento de temperatura, umidade e luminosidade com controle remoto dos LEDs
        e do LED RGB. O display de 7 segmentos mostra a temperatura atual localmente.
      </p>
    </section>

    <section class="grid">
      <article class="card">
        <div class="label">Temperatura</div>
        <div class="value" id="tempValue">--</div>
      </article>
      <article class="card">
        <div class="label">Umidade</div>
        <div class="value" id="humValue">--</div>
      </article>
      <article class="card">
        <div class="label">Luminosidade</div>
        <div class="value" id="ldrValue">--</div>
      </article>
      <article class="card">
        <div class="label">Display</div>
        <div class="value" id="displayValue">--</div>
      </article>
    </section>

    <section class="controls">
      <article class="card">
        <div class="label">LEDs Digitais</div>
        <div class="button-row">
          <button class="btn-red" onclick="toggleLed(0)">LED Vermelho</button>
          <button class="btn-green" onclick="toggleLed(1)">LED Verde</button>
          <button class="btn-blue" onclick="toggleLed(2)">LED Azul</button>
        </div>
        <div class="status-list">
          <div class="status-item"><span>Vermelho</span><span class="badge badge-off" id="led0">OFF</span></div>
          <div class="status-item"><span>Verde</span><span class="badge badge-off" id="led1">OFF</span></div>
          <div class="status-item"><span>Azul</span><span class="badge badge-off" id="led2">OFF</span></div>
        </div>
      </article>

      <article class="card">
        <div class="label">LED RGB</div>
        <div class="slider-wrap">
          <label class="slider-group">R <input id="sliderR" type="range" min="0" max="255" value="0" oninput="updatePreview()" onchange="sendRgb()"></label>
          <label class="slider-group">G <input id="sliderG" type="range" min="0" max="255" value="0" oninput="updatePreview()" onchange="sendRgb()"></label>
          <label class="slider-group">B <input id="sliderB" type="range" min="0" max="255" value="0" oninput="updatePreview()" onchange="sendRgb()"></label>
        </div>
        <div class="color-preview" id="preview"></div>
      </article>
    </section>

    <p class="footer-note" id="statusLine">Aguardando leitura dos sensores...</p>
  </main>

  <script>
    function badge(on) {
      return on
        ? '<span class="badge badge-on">ON</span>'
        : '<span class="badge badge-off">OFF</span>';
    }

    function updatePreview() {
      const r = document.getElementById('sliderR').value;
      const g = document.getElementById('sliderG').value;
      const b = document.getElementById('sliderB').value;
      document.getElementById('preview').style.background = `rgb(${r}, ${g}, ${b})`;
    }

    async function loadData() {
      try {
        const response = await fetch('/api/status');
        const data = await response.json();

        document.getElementById('tempValue').textContent = data.temperatura === null ? '--' : `${data.temperatura.toFixed(1)} C`;
        document.getElementById('humValue').textContent = data.umidade === null ? '--' : `${data.umidade.toFixed(1)} %`;
        document.getElementById('ldrValue').textContent = `${data.luminosidade_percentual} %`;
        document.getElementById('displayValue').textContent = data.display.toString().padStart(2, '0');

        document.getElementById('led0').outerHTML = badge(data.leds.vermelho).replace('badge', 'badge').replace('ON', data.leds.vermelho ? 'ON' : 'OFF');
        document.getElementById('led1').outerHTML = badge(data.leds.verde).replace('badge', 'badge').replace('ON', data.leds.verde ? 'ON' : 'OFF');
        document.getElementById('led2').outerHTML = badge(data.leds.azul).replace('badge', 'badge').replace('ON', data.leds.azul ? 'ON' : 'OFF');

        document.getElementById('sliderR').value = data.rgb.r;
        document.getElementById('sliderG').value = data.rgb.g;
        document.getElementById('sliderB').value = data.rgb.b;
        updatePreview();

        document.getElementById('statusLine').textContent =
          data.temperatura === null
            ? 'DHT11 sem leitura valida no momento.'
            : `Ultima atualizacao: temperatura ${data.temperatura.toFixed(1)} C, umidade ${data.umidade.toFixed(1)} %, luminosidade ${data.luminosidade_percentual} %.`;
      } catch (error) {
        document.getElementById('statusLine').textContent = 'Falha ao buscar dados do ESP32.';
      }
    }

    async function toggleLed(index) {
      await fetch(`/api/led?index=${index}`, { method: 'POST' });
      loadData();
    }

    async function sendRgb() {
      const r = document.getElementById('sliderR').value;
      const g = document.getElementById('sliderG').value;
      const b = document.getElementById('sliderB').value;
      await fetch(`/api/rgb?r=${r}&g=${g}&b=${b}`, { method: 'POST' });
      loadData();
    }

    updatePreview();
    loadData();
    setInterval(loadData, 2000);
  </script>
</body>
</html>
  )rawliteral";
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Conectando ao Wi-Fi");
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Wi-Fi conectado. IP: ");
    Serial.println(WiFi.localIP());
    return;
  }

  Serial.println("Falha ao conectar no Wi-Fi configurado. Iniciando modo Access Point.");
  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  Serial.print("Rede criada: ");
  Serial.println(AP_SSID);
  Serial.print("IP do AP: ");
  Serial.println(WiFi.softAPIP());
}

void configureServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("HTTP GET /");
    request->send(200, "text/html; charset=utf-8", buildPage());
  });

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("HTTP GET /api/status");
    request->send(200, "application/json", buildJson());
  });

  server.on("/api/led", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("HTTP POST /api/led");

    if (!request->hasParam("index")) {
      request->send(400, "application/json", "{\"ok\":false,\"erro\":\"Parametro index ausente\"}");
      return;
    }

    int index = request->getParam("index")->value().toInt();
    if (index < 0 || index > 2) {
      request->send(400, "application/json", "{\"ok\":false,\"erro\":\"Indice de LED invalido\"}");
      return;
    }

    setDiscreteLed(index, !ledStates[index]);
    request->send(200, "application/json", "{\"ok\":true}");
  });

  server.on("/api/rgb", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("HTTP POST /api/rgb");

    if (!request->hasParam("r") || !request->hasParam("g") || !request->hasParam("b")) {
      request->send(400, "application/json", "{\"ok\":false,\"erro\":\"Parametros RGB incompletos\"}");
      return;
    }

    rgbValues[0] = constrain(request->getParam("r")->value().toInt(), 0, 255);
    rgbValues[1] = constrain(request->getParam("g")->value().toInt(), 0, 255);
    rgbValues[2] = constrain(request->getParam("b")->value().toInt(), 0, 255);
    applyRgb();

    request->send(200, "application/json", "{\"ok\":true}");
  });

  server.onNotFound([](AsyncWebServerRequest *request) {
    Serial.print("404 em ");
    Serial.println(request->url());
    request->send(404, "application/json", "{\"ok\":false,\"erro\":\"Rota nao encontrada\"}");
  });

  server.begin();
}

void configurePins() {
  for (uint8_t i = 0; i < 3; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    setDiscreteLed(i, false);
  }

  for (uint8_t i = 0; i < 8; i++) {
    pinMode(SEGMENT_PINS[i], OUTPUT);
    writeSegment(SEGMENT_PINS[i], false);
  }

  for (uint8_t i = 0; i < 2; i++) {
    pinMode(DIGIT_PINS[i], OUTPUT);
  }
  disableAllDigits();

  ledcAttach(RGB_RED, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttach(RGB_GREEN, PWM_FREQUENCY, PWM_RESOLUTION);
  ledcAttach(RGB_BLUE, PWM_FREQUENCY, PWM_RESOLUTION);
  applyRgb();

  analogReadResolution(12);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.println("Aula 05 - Atividade");

  configurePins();
  dht.begin();
  connectWiFi();
  currentData = readSensors();
  if (currentData.dhtOk) {
    displayValue = constrain(static_cast<int>(currentData.temperature + 0.5f), 0, 99);
  }
  configureServer();

  Serial.println("Servidor AsyncWebServer iniciado.");
}

void loop() {
  updateSensorData();
  refreshDisplay();
}
