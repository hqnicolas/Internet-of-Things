#include <DFRobot_DHT11.h>
 
#define DHT11_PIN 33 //VP
#define LDR_PIN 39 //VN (Renomeado LDR para LDR_PIN para clareza)

// Declare o objeto DFRobot_DHT11
DFRobot_DHT11 DHT;

// Esta função é executada apenas uma vez ao iniciar o algoritmo, antes do loop
void setup() {
  Serial.begin(115200);

  // O pino LDR é uma entrada analógica, não é necessário definir pinMode para analogRead
  // No entanto, definir explicitamente como INPUT pode ser uma boa prática.
  pinMode(LDR_PIN, INPUT); 
}

// Esta função repetirá indefinidamente
void loop() {
  // Leia os dados do sensor DHT11
  DHT.read(DHT11_PIN);

  // Leia os dados do sensor LDR
  int ldrValue = analogRead(LDR_PIN);

  // Imprima todos os valores dos sensores no Monitor Serial
  Serial.print("Temperatura: ");
  Serial.print(DHT.temperature);
  Serial.print(" °C\tUmidade: ");
  Serial.print(DHT.humidity);
  Serial.print(" %\tLuminosidade: ");
  Serial.println(ldrValue); // Valores LDR normalmente variam de 0 a 4095 no ESP32

  delay(2000); // Aguarde 2 segundos antes da próxima leitura
}