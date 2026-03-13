/*
write:
GET https://api.thingspeak.com/update?api_key=ALCJVVDR0XUOAG7O&field1=0

Read a Channel Field
GET https://api.thingspeak.com/channels/3297933/fields/1.json?api_key=BAZBX91PIXU0DOX1&results=2

Read Channel Status Updates
GET https://api.thingspeak.com/channels/3297933/status.json?api_key=BAZBX91PIXU0DOX1

*/

#include <DFRobot_DHT11.h>
#include <WiFi.h> // Inclui a biblioteca para conexão Wi-Fi
#include <ThingSpeak.h> // Inclui a biblioteca para comunicação com ThingSpeak
 
#define DHT11_PIN 33 //VP
#define LDR_PIN 39 //VN (Renomeado LDR para LDR_PIN para clareza)

// Declare o objeto DFRobot_DHT11
DFRobot_DHT11 DHT;

// --- Configurações de Wi-Fi ---
const char* ssid = "SATC_grupo_4"; // Nome da rede Wi-Fi (SSID)
const char* password = "12345678"; // Senha da rede Wi-Fi

// --- Configurações do ThingSpeak ---
// Substitua estes valores pelo seu Channel ID e Write API Key do ThingSpeak
unsigned long myChannelNumber = 3297933; // Ex: 3297933
const char* myWriteAPIKey = "ALCJVVDR0XUOAG7O"; // Adicione esta linha com sua Write API Key

WiFiClient client; // Objeto para a conexão Wi-Fi com o servidor ThingSpeak

void conectarWiFi() {
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password); // Conexão WPA2 padrão

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
  conectarWiFi(); // Chama a função de conexão Wi-Fi

  // --- Inicializa o ThingSpeak ---
  ThingSpeak.begin(client);
}

// Esta função repetirá indefinidamente
void loop() {
  // Verifique se a conexão Wi-Fi foi perdida e tente reconectar
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi desconectado. Tentando reconectar...");
    conectarWiFi(); // Reutiliza a função de conexão Wi-Fi
  }

  // Leia os dados do sensor DHT11
  DHT.read(DHT11_PIN); // Realiza a leitura do sensor
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