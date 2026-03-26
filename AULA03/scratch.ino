#include <DFRobot_DHT11.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

#define DHT11_PIN 33
#define LDR_PIN 39

DFRobot_DHT11 DHT;

const char* ssid = "Turma IOT";
const char* password = "12345678";

unsigned long myChannelNumber = 3297933;
const char* myWriteAPIKey = "ALCJVVDR0XUOAG7O";

WiFiClient client;

const char* city = "Criciuma";
const char* apiKey = "4c0156bd60f3c1b11a3e8ded166dcd99";

// Função para conectar ao Wi-Fi
void conectarWiFi() {
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(ssid);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 60) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Wi-Fi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Falha ao conectar no Wi-Fi.");
  }
}

float obterTemperaturaExterna() {
  HTTPClient http;
  String url = "http://api.openweathermap.org/data/2.5/weather?q=";
  url += city;
  url += "&appid=";
  url += apiKey;
  url += "&units=metric";

  http.begin(url);
  int httpCode = http.GET();
  float tempExterna = -100;

  if (httpCode > 0) {
    String payload = http.getString();

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);

    if (!error) {
      tempExterna = doc["main"]["temp"];
    } else {
      Serial.println("Erro ao interpretar JSON");
    }
  } else {
    Serial.print("Erro na requisição HTTP: ");
    Serial.println(httpCode);
  }

  http.end();
  return tempExterna;
}

void setup() {
  Serial.begin(9600);
  pinMode(LDR_PIN, INPUT);

  conectarWiFi();
  ThingSpeak.begin(client);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi desconectado. Tentando reconectar...");
    conectarWiFi();
  }

   DHT.read(DHT11_PIN);
  float tempInterna = DHT.temperature;
  float umidade = DHT.humidity;

  int ldrValue = analogRead(LDR_PIN);

  float tempExterna = obterTemperaturaExterna();


  Serial.print("Temp Interna: "); Serial.print(tempInterna); Serial.print(" °C\t");
  Serial.print("Umidade: "); Serial.print(umidade); Serial.print(" %\t");
  Serial.print("Temp Externa: "); Serial.print(tempExterna); Serial.print(" °C\t");
  Serial.print("LDR: "); Serial.println(ldrValue);

 
  ThingSpeak.setField(1, tempInterna);
  ThingSpeak.setField(2, tempExterna);
  ThingSpeak.setField(3, umidade);
  ThingSpeak.setField(4, ldrValue);

  int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (httpCode == 200) {
    Serial.println("Dados publicados no ThingSpeak com sucesso!");
  } else {
    Serial.print("Erro ao publicar dados: ");
    Serial.println(httpCode);
  }

  delay(20000);
}