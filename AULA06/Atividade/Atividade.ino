#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap-simple.h>

const char *WIFI_SSID = "SEU_WIFI";
const char *WIFI_PASSWORD = "SUA_SENHA";

constexpr int LDR_PIN = 39;
constexpr int LED_PIN = 2;
constexpr int COAP_PORT = 5683;

constexpr unsigned long WIFI_CONNECT_TIMEOUT_MS = 20000;
constexpr unsigned long WIFI_RETRY_INTERVAL_MS = 5000;

WiFiUDP udp;
Coap coap(udp);

bool ledState = false;
bool wifiConnected = false;
unsigned long lastReconnectAttemptMs = 0;

void startWiFiConnection() {
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(WIFI_SSID);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
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

  if (lastReconnectAttemptMs == 0 ||
      millis() - lastReconnectAttemptMs >= WIFI_RETRY_INTERVAL_MS) {
    lastReconnectAttemptMs = millis();
    startWiFiConnection();
  }
}

int readLdrPercentage(int rawValue) {
  int lightPercentage = map(rawValue, 0, 4095, 0, 100);
  return constrain(lightPercentage, 0, 100);
}

void sendCoapTextResponse(CoapPacket &packet,
                          IPAddress ip,
                          int port,
                          const char *payload) {
  coap.sendResponse(
    ip,
    port,
    packet.messageid,
    payload,
    strlen(payload),
    COAP_CONTENT,
    COAP_TEXT_PLAIN,
    packet.token,
    packet.tokenlen
  );
}

void sendCoapJsonResponse(CoapPacket &packet,
                          IPAddress ip,
                          int port,
                          const char *payload) {
  coap.sendResponse(
    ip,
    port,
    packet.messageid,
    payload,
    strlen(payload),
    COAP_CONTENT,
    COAP_APPLICATION_JSON,
    packet.token,
    packet.tokenlen
  );
}

void callbackStatus(CoapPacket &packet, IPAddress ip, int port) {
  sendCoapTextResponse(packet, ip, port, "ESP32 CoAP online");
}

void callbackLdr(CoapPacket &packet, IPAddress ip, int port) {
  int rawValue = analogRead(LDR_PIN);
  int lightPercentage = readLdrPercentage(rawValue);

  char payload[64];
  snprintf(
    payload,
    sizeof(payload),
    "{\"raw\":%d,\"luz\":%d}",
    rawValue,
    lightPercentage
  );

  sendCoapJsonResponse(packet, ip, port, payload);
}

void callbackLed(CoapPacket &packet, IPAddress ip, int port) {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);

  const char *payload = ledState ? "{\"led\":\"on\"}" : "{\"led\":\"off\"}";
  sendCoapJsonResponse(packet, ip, port, payload);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(LDR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.println("AULA06 - CoAP com ESP32, LDR e LED");

  startWiFiConnection();
  wifiConnected = waitForWiFi(WIFI_CONNECT_TIMEOUT_MS);

  if (wifiConnected) {
    printWiFiStatus();
  } else {
    Serial.println("Falha na conexao inicial com o Wi-Fi.");
    Serial.println("O loop continuara tentando reconectar.");
  }

  coap.server(callbackStatus, "status");
  coap.server(callbackLdr, "ldr");
  coap.server(callbackLed, "led");
  coap.start(COAP_PORT);

  Serial.print("Servidor CoAP iniciado na porta ");
  Serial.println(COAP_PORT);
  Serial.println("Recursos disponiveis: /status, /ldr, /led");
}

void loop() {
  maintainWiFiConnection();
  coap.loop();
  delay(10);
}
