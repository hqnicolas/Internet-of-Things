#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <DFRobot_DHT11.h>

const char *WIFI_SSID = "SATC 2.4";
const char *EAP_IDENTITY = "aluno.123456@alunosatc.edu.br";
const char *EAP_USERNAME = "aluno.123456@alunosatc.edu.br";
const char *EAP_PASSWORD = "123456";

const char *MQTT_HOST = "broker.hivemq.com";
constexpr int MQTT_PORT = 8883;
const char *MQTT_USERNAME = "pomodoro.satc";
const char *MQTT_PASSWORD = "pomodoro.satc";
constexpr uint16_t MQTT_KEEP_ALIVE_SECONDS = 60;
constexpr uint16_t MQTT_SOCKET_TIMEOUT_SECONDS = 20;

constexpr int GROUP_ID = 7;

#define RELE 13
#define A 18
#define B 5
#define C 21
#define D 3
#define E 1
#define F 23
#define G 22
#define DP 19
#define DISPLAY1 16
#define DISPLAY2 17
#define DHT11_PIN 33
#define LDR_PIN 39

#define LED1 4
#define LED2 0
#define LED3 2
#define LED4 15
#define BOTAO1 4
#define BOTAO2 0
#define BOTAO3 2
#define BOTAO4 15

constexpr bool SHARE_LED_PINS_AS_BUTTONS = true;
constexpr bool DISPLAY_SEGMENT_ACTIVE_HIGH = true;
constexpr bool DISPLAY_DIGIT_ACTIVE_LOW = true;

constexpr unsigned long WIFI_CONNECT_TIMEOUT_MS = 20000;
constexpr unsigned long WIFI_RETRY_INTERVAL_MS = 5000;
constexpr unsigned long MQTT_RETRY_INTERVAL_MS = 5000;
constexpr unsigned long BUTTON_DEBOUNCE_MS = 60;
constexpr unsigned long SHARED_PIN_OUTPUT_TIME_US = 1500;
constexpr unsigned long SENSOR_READ_INTERVAL_MS = 2000;
constexpr unsigned long TELEMETRY_PUBLISH_INTERVAL_MS = 5000;
constexpr unsigned long STATUS_PUBLISH_INTERVAL_MS = 5000;
constexpr unsigned long DISPLAY_REFRESH_INTERVAL_US = 2500;

constexpr int DEFAULT_LUMINOSITY_THRESHOLD = 40;
constexpr int DEFAULT_HUMIDITY_ALERT_THRESHOLD = 30;
constexpr unsigned long DEFAULT_FOCUS_DURATION_MS = 25UL * 60UL * 1000UL;
constexpr unsigned long DEFAULT_PAUSE_DURATION_MS = 5UL * 60UL * 1000UL;

enum FocusState {
  FOCUS_IDLE,
  FOCUS_RUNNING,
  FOCUS_PAUSED
};

enum DisplayMode {
  DISPLAY_TEMPERATURE,
  DISPLAY_HUMIDITY,
  DISPLAY_LUMINOSITY,
  DISPLAY_TIMER
};

DFRobot_DHT11 dht;
WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);
WebServer server(80);

const int LED_PINS[4] = {LED1, LED2, LED3, LED4};
const int BUTTON_PINS[4] = {BOTAO1, BOTAO2, BOTAO3, BOTAO4};
const int SEGMENT_PINS[8] = {A, B, C, D, E, F, G, DP};
const int DISPLAY_PINS[2] = {DISPLAY1, DISPLAY2};

const byte DIGIT_PATTERNS[10][8] = {
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

bool ledStates[4] = {false, false, false, false};
bool buttonStableStates[4] = {HIGH, HIGH, HIGH, HIGH};
bool buttonLastReadings[4] = {HIGH, HIGH, HIGH, HIGH};
unsigned long buttonLastDebounceMs[4] = {0, 0, 0, 0};

bool wifiConnected = false;
bool serverStarted = false;
bool manualOverrideEnabled = false;
bool manualOverrideState = false;
bool lightState = false;
bool dhtOk = false;

float temperatureC = 0.0f;
float humidityPercent = 0.0f;
int ldrRaw = 0;
int ldrPercent = 0;

int luminosityThreshold = DEFAULT_LUMINOSITY_THRESHOLD;
int humidityAlertThreshold = DEFAULT_HUMIDITY_ALERT_THRESHOLD;
unsigned long focusDurationMs = DEFAULT_FOCUS_DURATION_MS;
unsigned long pauseDurationMs = DEFAULT_PAUSE_DURATION_MS;

FocusState focusState = FOCUS_IDLE;
DisplayMode displayMode = DISPLAY_LUMINOSITY;

unsigned long focusStateChangedAtMs = 0;
unsigned long lastSensorReadMs = 0;
unsigned long lastTelemetryPublishMs = 0;
unsigned long lastStatusPublishMs = 0;
unsigned long lastWiFiReconnectAttemptMs = 0;
unsigned long lastMqttReconnectAttemptMs = 0;
unsigned long lastDisplayRefreshUs = 0;

char topicTemperature[48];
char topicHumidity[48];
char topicLuminosity[48];
char topicLightCommand[48];
char topicFocusStatus[48];
char topicAlertStatus[48];

String lastPublishedFocus = "";
String lastPublishedAlert = "";

void buildTopics() {
  snprintf(topicTemperature, sizeof(topicTemperature), "satc/g%d/telemetry/temperature", GROUP_ID);
  snprintf(topicHumidity, sizeof(topicHumidity), "satc/g%d/telemetry/humidity", GROUP_ID);
  snprintf(topicLuminosity, sizeof(topicLuminosity), "satc/g%d/telemetry/luminosity", GROUP_ID);
  snprintf(topicLightCommand, sizeof(topicLightCommand), "satc/g%d/cmd/light", GROUP_ID);
  snprintf(topicFocusStatus, sizeof(topicFocusStatus), "satc/g%d/status/focus", GROUP_ID);
  snprintf(topicAlertStatus, sizeof(topicAlertStatus), "satc/g%d/status/alert", GROUP_ID);
}

bool payloadMeansOn(const String &payload) {
  return payload.equalsIgnoreCase("ON") ||
         payload.equalsIgnoreCase("1") ||
         payload.equalsIgnoreCase("TRUE");
}

bool payloadMeansOff(const String &payload) {
  return payload.equalsIgnoreCase("OFF") ||
         payload.equalsIgnoreCase("0") ||
         payload.equalsIgnoreCase("FALSE");
}

const char *focusStateToText(FocusState state) {
  switch (state) {
    case FOCUS_RUNNING:
      return "FOCUS";
    case FOCUS_PAUSED:
      return "PAUSE";
    default:
      return "IDLE";
  }
}

String buildAlertState() {
  if (!dhtOk) {
    return "WARN";
  }
  if (humidityPercent < humidityAlertThreshold) {
    return "ALERT";
  }
  return "OK";
}

void configureSharedPinsAsInput() {
  for (int i = 0; i < 4; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
}

void setLedState(int index, bool state) {
  ledStates[index] = state;

  if (!SHARE_LED_PINS_AS_BUTTONS) {
    digitalWrite(LED_PINS[index], state ? HIGH : LOW);
  }
}

void refreshSharedLedOutputs() {
  if (!SHARE_LED_PINS_AS_BUTTONS) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], ledStates[i] ? HIGH : LOW);
  }

  delayMicroseconds(SHARED_PIN_OUTPUT_TIME_US);
  configureSharedPinsAsInput();
}

void applySegmentLevel(int pin, bool active) {
  digitalWrite(pin, active == DISPLAY_SEGMENT_ACTIVE_HIGH ? HIGH : LOW);
}

void applyDigitLevel(int pin, bool active) {
  bool level = active ? !DISPLAY_DIGIT_ACTIVE_LOW : DISPLAY_DIGIT_ACTIVE_LOW;
  digitalWrite(pin, level ? HIGH : LOW);
}

void disableDisplays() {
  for (int i = 0; i < 2; i++) {
    applyDigitLevel(DISPLAY_PINS[i], false);
  }
}

void writeDigitPattern(int value, bool decimalPoint) {
  if (value < 0 || value > 9) {
    for (int i = 0; i < 8; i++) {
      applySegmentLevel(SEGMENT_PINS[i], false);
    }
    return;
  }

  for (int i = 0; i < 7; i++) {
    applySegmentLevel(SEGMENT_PINS[i], DIGIT_PATTERNS[value][i] == 1);
  }

  applySegmentLevel(DP, decimalPoint);
}

int getDisplayValue() {
  switch (displayMode) {
    case DISPLAY_TEMPERATURE:
      return constrain((int)round(temperatureC), 0, 99);
    case DISPLAY_HUMIDITY:
      return constrain((int)round(humidityPercent), 0, 99);
    case DISPLAY_TIMER: {
      unsigned long elapsed = millis() - focusStateChangedAtMs;
      unsigned long duration = focusState == FOCUS_PAUSED ? pauseDurationMs : focusDurationMs;

      if (focusState == FOCUS_IDLE) {
        return 0;
      }

      unsigned long remaining = elapsed >= duration ? 0 : duration - elapsed;
      return constrain((int)(remaining / 60000UL), 0, 99);
    }
    case DISPLAY_LUMINOSITY:
    default:
      return constrain(ldrPercent, 0, 99);
  }
}

void refreshDisplay() {
  unsigned long nowUs = micros();

  if (nowUs - lastDisplayRefreshUs < DISPLAY_REFRESH_INTERVAL_US) {
    return;
  }

  lastDisplayRefreshUs = nowUs;

  static int activeDigit = 0;
  int displayValue = getDisplayValue();
  int tens = displayValue / 10;
  int units = displayValue % 10;

  disableDisplays();

  if (activeDigit == 0) {
    writeDigitPattern(tens, false);
    applyDigitLevel(DISPLAY1, true);
  } else {
    writeDigitPattern(units, false);
    applyDigitLevel(DISPLAY2, true);
  }

  activeDigit = 1 - activeDigit;
}

void setRelayState(bool enabled) {
  lightState = enabled;
  digitalWrite(RELE, enabled ? HIGH : LOW);
}

void updateStatusLeds() {
  setLedState(0, focusState == FOCUS_RUNNING);
  setLedState(1, focusState == FOCUS_PAUSED);
  setLedState(2, wifiConnected && mqttClient.connected());
  setLedState(3, buildAlertState() != "OK");
}

void applyLightingRule() {
  bool automaticLight = focusState == FOCUS_RUNNING && ldrPercent < luminosityThreshold;
  bool desiredState = manualOverrideEnabled ? manualOverrideState : automaticLight;
  setRelayState(desiredState);
}

void publishStatusIfNeeded(bool force = false) {
  if (!mqttClient.connected()) {
    return;
  }

  String focusText = focusStateToText(focusState);
  String alertText = buildAlertState();

  if (force || focusText != lastPublishedFocus) {
    mqttClient.publish(topicFocusStatus, focusText.c_str(), true);
    lastPublishedFocus = focusText;
  }

  if (force || alertText != lastPublishedAlert) {
    mqttClient.publish(topicAlertStatus, alertText.c_str(), true);
    lastPublishedAlert = alertText;
  }
}

void readSensors() {
  dht.read(DHT11_PIN);
  ldrRaw = analogRead(LDR_PIN);
  ldrPercent = map(ldrRaw, 0, 4095, 100, 0);
  ldrPercent = constrain(ldrPercent, 0, 100);

  float nextTemperature = dht.temperature;
  float nextHumidity = dht.humidity;
  dhtOk = nextTemperature > -40.0f && nextTemperature < 100.0f &&
          nextHumidity >= 0.0f && nextHumidity <= 100.0f;

  if (dhtOk) {
    temperatureC = nextTemperature;
    humidityPercent = nextHumidity;
  }

  updateStatusLeds();
  applyLightingRule();
}

void publishTelemetry() {
  if (!mqttClient.connected()) {
    return;
  }

  char buffer[16];

  dtostrf(temperatureC, 0, 1, buffer);
  mqttClient.publish(topicTemperature, buffer, true);

  dtostrf(humidityPercent, 0, 1, buffer);
  mqttClient.publish(topicHumidity, buffer, true);

  snprintf(buffer, sizeof(buffer), "%d", ldrPercent);
  mqttClient.publish(topicLuminosity, buffer, true);
}

void setFocusState(FocusState nextState) {
  if (focusState == nextState) {
    return;
  }

  focusState = nextState;
  focusStateChangedAtMs = millis();
  updateStatusLeds();
  applyLightingRule();
  publishStatusIfNeeded(true);
}

void toggleDisplayMode() {
  switch (displayMode) {
    case DISPLAY_TEMPERATURE:
      displayMode = DISPLAY_HUMIDITY;
      break;
    case DISPLAY_HUMIDITY:
      displayMode = DISPLAY_LUMINOSITY;
      break;
    case DISPLAY_LUMINOSITY:
      displayMode = DISPLAY_TIMER;
      break;
    case DISPLAY_TIMER:
    default:
      displayMode = DISPLAY_TEMPERATURE;
      break;
  }
}

void toggleManualOverride() {
  if (manualOverrideEnabled) {
    manualOverrideEnabled = false;
  } else {
    manualOverrideEnabled = true;
    manualOverrideState = !lightState;
  }

  applyLightingRule();
}

void handleButtonPress(int buttonIndex) {
  switch (buttonIndex) {
    case 0:
      if (focusState == FOCUS_IDLE || focusState == FOCUS_PAUSED) {
        setFocusState(FOCUS_RUNNING);
      } else {
        setFocusState(FOCUS_PAUSED);
      }
      break;
    case 1:
      manualOverrideEnabled = false;
      setFocusState(FOCUS_IDLE);
      break;
    case 2:
      toggleDisplayMode();
      break;
    case 3:
      toggleManualOverride();
      break;
  }
}

void processButtons() {
  if (!SHARE_LED_PINS_AS_BUTTONS) {
    return;
  }

  for (int i = 0; i < 4; i++) {
    bool reading = digitalRead(BUTTON_PINS[i]);

    if (reading != buttonLastReadings[i]) {
      buttonLastReadings[i] = reading;
      buttonLastDebounceMs[i] = millis();
    }

    if (millis() - buttonLastDebounceMs[i] < BUTTON_DEBOUNCE_MS) {
      continue;
    }

    if (reading != buttonStableStates[i]) {
      buttonStableStates[i] = reading;

      if (buttonStableStates[i] == LOW) {
        handleButtonPress(i);
      }
    }
  }
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
  char messageBuffer[32];
  unsigned int copyLength = min(length, sizeof(messageBuffer) - 1);

  memcpy(messageBuffer, payload, copyLength);
  messageBuffer[copyLength] = '\0';

  String message = String(messageBuffer);

  if (strcmp(topic, topicLightCommand) == 0) {
    if (payloadMeansOn(message)) {
      manualOverrideEnabled = true;
      manualOverrideState = true;
      applyLightingRule();
    } else if (payloadMeansOff(message)) {
      manualOverrideEnabled = true;
      manualOverrideState = false;
      applyLightingRule();
    }
  }
}

void subscribeTopics() {
  mqttClient.subscribe(topicLightCommand);
}

void startWiFiConnection() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
}

bool waitForWiFi(unsigned long timeoutMs) {
  unsigned long startMs = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startMs < timeoutMs) {
    delay(500);
  }
  return WiFi.status() == WL_CONNECTED;
}

void startHttpServer();

void maintainWiFiConnection() {
  wl_status_t status = WiFi.status();

  if (status == WL_CONNECTED) {
    if (!wifiConnected) {
      wifiConnected = true;
      startHttpServer();
    }
    return;
  }

  if (wifiConnected) {
    wifiConnected = false;
    serverStarted = false;
  }

  if (lastWiFiReconnectAttemptMs == 0 ||
      millis() - lastWiFiReconnectAttemptMs >= WIFI_RETRY_INTERVAL_MS) {
    lastWiFiReconnectAttemptMs = millis();
    startWiFiConnection();
  }
}

bool connectToMqttBroker() {
  String clientId = "ESP32-G" + String(GROUP_ID) + "-" + String((uint32_t)ESP.getEfuseMac(), HEX);

  bool connected = false;

  if (strlen(MQTT_USERNAME) > 0) {
    connected = mqttClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD);
  } else {
    connected = mqttClient.connect(clientId.c_str());
  }

  if (!connected) {
    return false;
  }

  subscribeTopics();
  publishStatusIfNeeded(true);
  return true;
}

void maintainMqttConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }

  if (mqttClient.connected()) {
    return;
  }

  if (lastMqttReconnectAttemptMs == 0 ||
      millis() - lastMqttReconnectAttemptMs >= MQTT_RETRY_INTERVAL_MS) {
    lastMqttReconnectAttemptMs = millis();
    connectToMqttBroker();
  }
}

String buildStatusJson() {
  String json = "{";
  json += "\"temperature\":";
  json += dhtOk ? String(temperatureC, 1) : "null";
  json += ",\"humidity\":";
  json += dhtOk ? String(humidityPercent, 1) : "null";
  json += ",\"luminosity\":";
  json += String(ldrPercent);
  json += ",\"luminosityRaw\":";
  json += String(ldrRaw);
  json += ",\"focus\":\"";
  json += focusStateToText(focusState);
  json += "\",\"alert\":\"";
  json += buildAlertState();
  json += "\",\"light\":";
  json += lightState ? "true" : "false";
  json += ",\"manualOverride\":";
  json += manualOverrideEnabled ? "true" : "false";
  json += ",\"thresholds\":{\"luminosity\":";
  json += String(luminosityThreshold);
  json += ",\"humidity\":";
  json += String(humidityAlertThreshold);
  json += "}}";
  return json;
}

String buildHtmlPage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Smart Desk IoT</title>
  <style>
    body { font-family: Arial, sans-serif; background: #f4f7fb; color: #10233a; margin: 0; padding: 24px; }
    main { max-width: 860px; margin: 0 auto; background: #ffffff; border-radius: 18px; padding: 24px; box-shadow: 0 14px 40px rgba(16, 35, 58, 0.12); }
    .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(160px, 1fr)); gap: 12px; margin-top: 18px; }
    .card { border: 1px solid #d8e1ec; border-radius: 14px; padding: 16px; background: #f8fbff; }
    .label { font-size: 0.9rem; color: #4c6480; }
    .value { font-size: 1.7rem; font-weight: 700; margin-top: 8px; }
    code { background: #eef4fb; padding: 2px 6px; border-radius: 6px; }
  </style>
</head>
<body>
  <main>
    <h1>Smart Desk Ergonomica IoT</h1>
    <p>Status local do ESP32. Ajustes rapidos: <code>/config?lux=40</code> e <code>/config?humidity=30</code>.</p>
    <div class="grid">
  )rawliteral";

  html += "<section class=\"card\"><div class=\"label\">Temperatura</div><div class=\"value\">";
  html += dhtOk ? String(temperatureC, 1) + " C" : "--";
  html += "</div></section>";

  html += "<section class=\"card\"><div class=\"label\">Umidade</div><div class=\"value\">";
  html += dhtOk ? String(humidityPercent, 1) + " %" : "--";
  html += "</div></section>";

  html += "<section class=\"card\"><div class=\"label\">Luminosidade</div><div class=\"value\">";
  html += String(ldrPercent) + " %";
  html += "</div></section>";

  html += "<section class=\"card\"><div class=\"label\">Foco</div><div class=\"value\">";
  html += focusStateToText(focusState);
  html += "</div></section>";

  html += "<section class=\"card\"><div class=\"label\">Luminaria</div><div class=\"value\">";
  html += lightState ? "ON" : "OFF";
  html += "</div></section>";

  html += "<section class=\"card\"><div class=\"label\">Alerta</div><div class=\"value\">";
  html += buildAlertState();
  html += "</div></section>";

  html += R"rawliteral(
    </div>
  </main>
</body>
</html>
  )rawliteral";

  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=utf-8", buildHtmlPage());
}

void handleStatus() {
  server.send(200, "application/json", buildStatusJson());
}

void handleConfig() {
  if (server.hasArg("lux")) {
    luminosityThreshold = constrain(server.arg("lux").toInt(), 0, 100);
  }

  if (server.hasArg("humidity")) {
    humidityAlertThreshold = constrain(server.arg("humidity").toInt(), 0, 100);
  }

  server.send(200, "application/json", buildStatusJson());
}

void handleNotFound() {
  server.send(404, "text/plain", "Rota nao encontrada.");
}

void startHttpServer() {
  if (serverStarted) {
    return;
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/config", HTTP_GET, handleConfig);
  server.onNotFound(handleNotFound);
  server.begin();
  serverStarted = true;
}

void initializeDisplayHardware() {
  for (int i = 0; i < 8; i++) {
    pinMode(SEGMENT_PINS[i], OUTPUT);
    applySegmentLevel(SEGMENT_PINS[i], false);
  }

  for (int i = 0; i < 2; i++) {
    pinMode(DISPLAY_PINS[i], OUTPUT);
    applyDigitLevel(DISPLAY_PINS[i], false);
  }
}

void initializeButtonsAndLeds() {
  for (int i = 0; i < 4; i++) {
    if (SHARE_LED_PINS_AS_BUTTONS) {
      pinMode(BUTTON_PINS[i], INPUT_PULLUP);
      buttonLastReadings[i] = digitalRead(BUTTON_PINS[i]);
      buttonStableStates[i] = buttonLastReadings[i];
      ledStates[i] = false;
    } else {
      pinMode(LED_PINS[i], OUTPUT);
      setLedState(i, false);
    }
  }

  if (SHARE_LED_PINS_AS_BUTTONS) {
    refreshSharedLedOutputs();
  }
}

void handleFocusTimers() {
  if (focusState == FOCUS_IDLE) {
    return;
  }

  unsigned long elapsed = millis() - focusStateChangedAtMs;

  if (focusState == FOCUS_RUNNING && elapsed >= focusDurationMs) {
    setFocusState(FOCUS_PAUSED);
  } else if (focusState == FOCUS_PAUSED && elapsed >= pauseDurationMs) {
    setFocusState(FOCUS_RUNNING);
  }
}

void setup() {
  delay(200);

  pinMode(RELE, OUTPUT);
  setRelayState(false);

  analogReadResolution(12);
  pinMode(LDR_PIN, INPUT);

  initializeDisplayHardware();
  initializeButtonsAndLeds();
  buildTopics();
  updateStatusLeds();

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  mqttClient.setKeepAlive(MQTT_KEEP_ALIVE_SECONDS);
  mqttClient.setSocketTimeout(MQTT_SOCKET_TIMEOUT_SECONDS);
  espClient.setInsecure();

  startWiFiConnection();
  wifiConnected = waitForWiFi(WIFI_CONNECT_TIMEOUT_MS);

  if (wifiConnected) {
    startHttpServer();
    connectToMqttBroker();
  }

  focusStateChangedAtMs = millis();
}

void loop() {
  refreshDisplay();
  refreshSharedLedOutputs();
  maintainWiFiConnection();
  maintainMqttConnection();
  mqttClient.loop();
  server.handleClient();
  processButtons();
  handleFocusTimers();

  unsigned long now = millis();

  if (now - lastSensorReadMs >= SENSOR_READ_INTERVAL_MS) {
    lastSensorReadMs = now;
    readSensors();
  }

  if (now - lastTelemetryPublishMs >= TELEMETRY_PUBLISH_INTERVAL_MS) {
    lastTelemetryPublishMs = now;
    publishTelemetry();
  }

  if (now - lastStatusPublishMs >= STATUS_PUBLISH_INTERVAL_MS) {
    lastStatusPublishMs = now;
    publishStatusIfNeeded();
  }

  delay(5);
}