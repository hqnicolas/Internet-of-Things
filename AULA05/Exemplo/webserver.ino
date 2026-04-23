/*
Download Libraries:
https://github.com/me-no-dev/asynctcp
https://github.com/me-no-dev/espasyncwebserver
*/


#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

#define DHTPIN 33    // Pino DHT11 na shield
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "SEU_WIFI";
const char* password = "SUA_SENHA";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("Cliente conectado: %u\n", client->id());
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(1000);
  Serial.println(WiFi.localIP());

  ws.onEvent(onWsEvent);
  server.addHandler(&ws);

  server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request){
    float temp = dht.readTemperature();
    String json = "{\"temperatura\":" + String(temp) + "}";
    request->send(200, "application/json", json);
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><body><h1>Dashboard IoT</h1><div id='temp'></div><script>const ws = new WebSocket('ws://' + location.hostname + '/ws'); ws.onmessage = (e) => document.getElementById('temp').innerHTML = e.data;</script></body></html>";
    request->send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  ws.cleanupClients();
  float temp = dht.readTemperature();
  if (!isnan(temp)) {
    String msg = "Temp: " + String(temp) + "°C";
    ws.textAll(msg);
  }
  delay(2000);
}