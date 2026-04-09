#include <WiFi.h>
#include <WebServer.h>

const char *SSID_AP = "ESP32_IoT_Aula04";
const char *PASSWORD_AP = "12345678";

WebServer server(80);

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Hello World</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: #f2f6fb;
      color: #1f2937;
      display: flex;
      align-items: center;
      justify-content: center;
      min-height: 100vh;
      margin: 0;
    }
    main {
      background: white;
      padding: 32px;
      border-radius: 16px;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.08);
      max-width: 520px;
      text-align: center;
    }
    h1 {
      margin-top: 0;
      color: #0f766e;
    }
    code {
      background: #e5e7eb;
      padding: 2px 6px;
      border-radius: 6px;
    }
  </style>
</head>
<body>
  <main>
    <h1>Hello World com ESP32</h1>
    <p>Servidor web sincronico em execucao com a biblioteca <code>WebServer.h</code>.</p>
    <p>Disciplina: Internet das Coisas</p>
    <p>Se voce esta vendo esta pagina, o ESP32 esta servindo HTML corretamente.</p>
  </main>
</body>
</html>
  )rawliteral";

  server.send(200, "text/html; charset=utf-8", html);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(SSID_AP, PASSWORD_AP);

  Serial.println();
  Serial.println("Exercicio 01 - ESP32 WebServer Hello World");
  Serial.print("Rede Wi-Fi criada: ");
  Serial.println(SSID_AP);
  Serial.print("IP do Access Point: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.begin();

  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  server.handleClient();
}
