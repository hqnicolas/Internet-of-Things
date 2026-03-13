#include <WiFi.h>
#include <Arduino.h>
#include <DFRobot_DHT11.h>
#include <ThingSpeak.h>

const char* WIFI_SSID = "SATC 2.4";

const char* EAP_IDENTITY = "aluno.12345@alunosatc.edu.br";
const char* EAP_USERNAME = "aluno.12345@alunosatc.edu.br";
const char* EAP_PASSWORD = "123456";

#define DHT_PIN 33
#define LDR_PIN 39

// Declare o objeto DHT11
DFRobot_DHT11 DHT;

// --- Configurações do ThingSpeak ---
// Substitua estes valores pelo seu Channel ID e Write API Key do ThingSpeak
unsigned long myChannelNumber = 3297933; // Ex: 3297933
const char* myWriteAPIKey = "ALCJVVDR0XUOAG7O"; // Read API: "BAZBX91PIXU0DOX1"

WiFiClient client; // Objeto para a conexão Wi-Fi com o servidor ThingSpeak

void conectarWiFiEnterprise() {
  Serial.print("Conectando ao WiFi Enterprise: ");
  Serial.println(WIFI_SSID);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);

  int tentativas = 0;

  while (WiFi.status() != WL_CONNECTED && tentativas < 60) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Falha ao conectar no WiFi.");
    // Considere adicionar um reset ou um loop infinito aqui para indicar falha
    // ou tentar novamente após um tempo maior.
  }
}

// Esta função é executada apenas uma vez ao iniciar o algoritmo, antes do loop
void setup() {
  Serial.begin(115200);

  // O pino LDR é uma entrada analógica, não é necessário definir pinMode para analogRead
  // No entanto, definir explicitamente como INPUT pode ser uma boa prática.
  pinMode(LDR_PIN, INPUT);

  // --- Conectando ao Wi-Fi ---
  conectarWiFiEnterprise();

  // --- Inicializa o ThingSpeak ---
  ThingSpeak.begin(client);
}

// Esta função repetirá indefinidamente
void loop() {
  // Verifique se a conexão Wi-Fi foi perdida e tente reconectar
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi desconectado. Tentando reconectar...");
    conectarWiFiEnterprise(); // Reutiliza a função de conexão WPA Enterprise
  }

  // Leia os dados do sensor DHT11
  DHT.read(DHT_PIN); // Realiza a leitura do sensor
  float temperature = DHT.temperature;
  float humidity = DHT.humidity;

  // Leia os dados do sensor LDR
  int ldrValue = analogRead(LDR_PIN);

  // Imprima todos os valores dos sensores no Monitor Serial
  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" °C\tUmidade: ");
  Serial.print(humidity);
  Serial.print(" %\tLuminosidade: ");
  Serial.println(ldrValue); // Valores LDR normalmente variam de 0 a 4095 no ESP32

  // --- Publica os dados no ThingSpeak ---
  // Define os campos do ThingSpeak com os valores dos sensores
  ThingSpeak.setField(1, temperature); // Campo 1 para Temperatura
  ThingSpeak.setField(2, humidity);    // Campo 2 para Umidade
  ThingSpeak.setField(3, ldrValue);        // Campo 3 para Luminosidade

  // Escreve os dados no canal do ThingSpeak
  int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (httpCode == 200) {
    Serial.println("Dados publicados no ThingSpeak com sucesso!");
  } else {
    Serial.print("Erro ao publicar dados no ThingSpeak. Código de erro: ");
    Serial.println(httpCode);
  }

  delay(20000); // Aguarde 20 segundos antes da próxima leitura (ThingSpeak tem limite de 15 segundos entre escritas)
}