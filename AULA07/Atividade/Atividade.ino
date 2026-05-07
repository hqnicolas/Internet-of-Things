#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

const char *WIFI_SSID = "SATC 2.4";
const char *EAP_IDENTITY = "aluno.123456@alunosatc.edu.br";
const char *EAP_USERNAME = "aluno.123456@alunosatc.edu.br";
const char *EAP_PASSWORD = "123456";

const char *MQTT_HOST = "broker.hivemq.com";
constexpr int MQTT_PORT = 8883;
const char *MQTT_USERNAME = "";
const char *MQTT_PASSWORD = "";
constexpr uint16_t MQTT_KEEP_ALIVE_SECONDS = 60;
constexpr uint16_t MQTT_SOCKET_TIMEOUT_SECONDS = 20;

constexpr int GROUP_ID = 7;

constexpr int LED1_PIN = 4;
constexpr int LED2_PIN = 0;
constexpr int LED3_PIN = 2;
constexpr int LED4_PIN = 15;

constexpr unsigned long WIFI_CONNECT_TIMEOUT_MS = 20000;
constexpr unsigned long WIFI_RETRY_INTERVAL_MS = 5000;
constexpr unsigned long MQTT_RETRY_INTERVAL_MS = 5000;

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

const int LED_PINS[4] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};
bool ledStates[4] = {false, false, false, false};

bool wifiConnected = false;
unsigned long lastWiFiReconnectAttemptMs = 0;
unsigned long lastMqttReconnectAttemptMs = 0;

char topicLed1[32];
char topicLed2[32];
char topicLed3[32];
char topicLed4[32];
char topicGroupAll[32];
char topicGlobalAll[32];

void buildTopics() {
  snprintf(topicLed1, sizeof(topicLed1), "satc/g%d/led1", GROUP_ID);
  snprintf(topicLed2, sizeof(topicLed2), "satc/g%d/led2", GROUP_ID);
  snprintf(topicLed3, sizeof(topicLed3), "satc/g%d/led3", GROUP_ID);
  snprintf(topicLed4, sizeof(topicLed4), "satc/g%d/led4", GROUP_ID);
  snprintf(topicGroupAll, sizeof(topicGroupAll), "satc/g%d/all", GROUP_ID);
  snprintf(topicGlobalAll, sizeof(topicGlobalAll), "satc/all/all");
}

void setLedState(int index, bool state) {
  ledStates[index] = state;
  digitalWrite(LED_PINS[index], state ? HIGH : LOW);
}

void setAllLeds(bool state) {
  for (int i = 0; i < 4; i++) {
    setLedState(i, state);
  }
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

void printLedStates() {
  Serial.print("Estados -> ");
  for (int i = 0; i < 4; i++) {
    Serial.print("LED");
    Serial.print(i + 1);
    Serial.print(":");
    Serial.print(ledStates[i] ? "ON" : "OFF");
    if (i < 3) {
      Serial.print(" | ");
    }
  }
  Serial.println();
}

void handleTopicCommand(const char *topic, const String &payload) {
  bool isOn = payloadMeansOn(payload);
  bool isOff = payloadMeansOff(payload);

  if (!isOn && !isOff) {
    Serial.print("Comando ignorado em ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(payload);
    Serial.println("Use ON ou OFF.");
    return;
  }

  if (strcmp(topic, topicLed1) == 0) {
    setLedState(0, isOn);
  } else if (strcmp(topic, topicLed2) == 0) {
    setLedState(1, isOn);
  } else if (strcmp(topic, topicLed3) == 0) {
    setLedState(2, isOn);
  } else if (strcmp(topic, topicLed4) == 0) {
    setLedState(3, isOn);
  } else if (strcmp(topic, topicGroupAll) == 0 || strcmp(topic, topicGlobalAll) == 0) {
    setAllLeds(isOn);
  } else {
    Serial.print("Topico nao tratado: ");
    Serial.println(topic);
    return;
  }

  Serial.print("Comando recebido em ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(payload);
  printLedStates();
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
  char messageBuffer[32];
  unsigned int copyLength = min(length, sizeof(messageBuffer) - 1);

  memcpy(messageBuffer, payload, copyLength);
  messageBuffer[copyLength] = '\0';

  handleTopicCommand(topic, String(messageBuffer));
}

void subscribeTopics() {
  mqttClient.subscribe(topicLed1);
  mqttClient.subscribe(topicLed2);
  mqttClient.subscribe(topicLed3);
  mqttClient.subscribe(topicLed4);
  mqttClient.subscribe(topicGroupAll);
  mqttClient.subscribe(topicGlobalAll);

  Serial.println("Topicos assinados:");
  Serial.println(topicLed1);
  Serial.println(topicLed2);
  Serial.println(topicLed3);
  Serial.println(topicLed4);
  Serial.println(topicGroupAll);
  Serial.println(topicGlobalAll);
}

void startWiFiConnection() {
  Serial.print("Conectando ao Wi-Fi Enterprise: ");
  Serial.println(WIFI_SSID);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
}

bool waitForWiFi(unsigned long timeoutMs) {
  unsigned long startMs = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startMs < timeoutMs) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  return WiFi.status() == WL_CONNECTED;
}

void printWiFiStatus() {
  Serial.println("Wi-Fi conectado.");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());
}

void maintainWiFiConnection() {
  wl_status_t status = WiFi.status();

  if (status == WL_CONNECTED) {
    if (!wifiConnected) {
      wifiConnected = true;
      printWiFiStatus();
    }
    return;
  }

  if (wifiConnected) {
    wifiConnected = false;
    Serial.println("Wi-Fi desconectado. Tentando reconectar...");
  }

  if (lastWiFiReconnectAttemptMs == 0 ||
      millis() - lastWiFiReconnectAttemptMs >= WIFI_RETRY_INTERVAL_MS) {
    lastWiFiReconnectAttemptMs = millis();
    startWiFiConnection();
  }
}

bool connectToMqttBroker() {
  String clientId = "ESP32-G" + String(GROUP_ID) + "-" + String((uint32_t)ESP.getEfuseMac(), HEX);

  Serial.print("Conectando ao broker MQTT ");
  Serial.print(MQTT_HOST);
  Serial.print(":");
  Serial.println(MQTT_PORT);

  bool connected = false;

  if (strlen(MQTT_USERNAME) > 0) {
    connected = mqttClient.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD);
  } else {
    connected = mqttClient.connect(clientId.c_str());
  }

  if (!connected) {
    Serial.print("Falha no MQTT. Codigo: ");
    Serial.println(mqttClient.state());
    return false;
  }

  Serial.println("MQTT conectado.");
  subscribeTopics();
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

void setup() {
  Serial.begin(115200);
  delay(200);

  for (int i = 0; i < 4; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    setLedState(i, false);
  }

  buildTopics();

  Serial.println();
  Serial.println("AULA07 - Controle Industrial com MQTT");
  Serial.print("Grupo configurado: G");
  Serial.println(GROUP_ID);

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  mqttClient.setKeepAlive(MQTT_KEEP_ALIVE_SECONDS);
  mqttClient.setSocketTimeout(MQTT_SOCKET_TIMEOUT_SECONDS);
  espClient.setInsecure();

  startWiFiConnection();
  wifiConnected = waitForWiFi(WIFI_CONNECT_TIMEOUT_MS);

  if (wifiConnected) {
    printWiFiStatus();
    connectToMqttBroker();
  } else {
    Serial.println("Falha na conexao inicial com o Wi-Fi.");
    Serial.println("O loop continuara tentando reconectar.");
  }
}

void loop() {
  maintainWiFiConnection();
  maintainMqttConnection();
  mqttClient.loop();
  delay(10);
}
