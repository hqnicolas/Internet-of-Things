#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

const char *WIFI_SSID = "SEU_WIFI";
const char *WIFI_PASSWORD = "SUA_SENHA";

WebServer server(80);

String getContentType(const String &path) {
  if (path.endsWith(".html")) {
    return "text/html; charset=utf-8";
  }
  if (path.endsWith(".css")) {
    return "text/css; charset=utf-8";
  }
  if (path.endsWith(".js")) {
    return "application/javascript; charset=utf-8";
  }
  if (path.endsWith(".json")) {
    return "application/json";
  }
  if (path.endsWith(".png")) {
    return "image/png";
  }
  if (path.endsWith(".jpg") || path.endsWith(".jpeg")) {
    return "image/jpeg";
  }
  if (path.endsWith(".svg")) {
    return "image/svg+xml";
  }
  return "text/plain; charset=utf-8";
}

bool handleFileRead(String path) {
  if (path.endsWith("/")) {
    path += "index.html";
  }

  if (!SPIFFS.exists(path)) {
    return false;
  }

  File file = SPIFFS.open(path, "r");
  server.streamFile(file, getContentType(path));
  file.close();
  return true;
}

void handleApiInfo() {
  String json = "{";
  json += "\"board\":\"ESP32 Dev Module\",";
  json += "\"storage\":\"SPIFFS\",";
  json += "\"status\":\"ok\",";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleNotFound() {
  if (handleFileRead(server.uri())) {
    return;
  }

  server.send(404, "text/plain", "Arquivo ou rota nao encontrada.");
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Conectando ao Wi-Fi");
  unsigned long startAttempt = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 20000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Conectado. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Falha ao conectar no Wi-Fi. Ajuste SSID e senha.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.println("Exercicio 04 - SPIFFS WebServer");

  if (!SPIFFS.begin(true)) {
    Serial.println("Erro ao iniciar o SPIFFS.");
    return;
  }

  Serial.println("SPIFFS montado com sucesso.");

  connectToWiFi();

  server.on("/api/info", HTTP_GET, handleApiInfo);
  server.on("/", HTTP_GET, []() {
    if (!handleFileRead("/index.html")) {
      server.send(500, "text/plain", "index.html nao encontrado no SPIFFS.");
    }
  });
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  server.handleClient();
}
