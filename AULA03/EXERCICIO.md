Boa — você já tem uma base bem avançada com esse código da Aula 02. Vamos organizar isso de forma clara para você executar a **Aula 03 sem retrabalho**.

---

# 🧠 Visão geral do que a atividade pede

Você precisa:

1. Conectar no Wi-Fi (simples, não Enterprise)
2. Ler **temperatura interna (DHT11)**
3. Buscar **temperatura externa via API (OpenWeather)**
4. Enviar **ambas para o ThingSpeak**

---

# ✅ O que você **já tem pronto (Aula 02)**

Seu código já resolve boa parte:

### ✔ Conexão Wi-Fi
- Já implementada (WPA2 Enterprise)
- ⚠️ Mas precisa adaptar para Wi-Fi comum

### ✔ Leitura do DHT11
- Já funcionando:
```cpp
DHT.read(DHT_PIN);
float temperature = DHT.temperature;
float humidity = DHT.humidity;
```

### ✔ Envio para o ThingSpeak
- Já implementado:
```cpp
ThingSpeak.setField(1, temperature);
ThingSpeak.writeFields(...);
```

### ✔ Estrutura geral
- `setup()` e `loop()` bem organizados
- Reconexão Wi-Fi já tratada

👉 Ou seja: **~70% do projeto já está pronto**

---

# ❗ O que você precisa adicionar/modificar

## 1. 🔌 Trocar Wi-Fi Enterprise por Wi-Fi comum

### Hoje:
```cpp
WiFi.begin(WIFI_SSID, WPA2_AUTH_PEAP, ...);
```

### Vai virar:
```cpp
const char* WIFI_SSID = "Turma IOT";
const char* WIFI_PASSWORD = "12345678";

WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
```

👉 Simplifica bastante

---

## 2. 🌐 Fazer requisição HTTP (OpenWeather)

Você ainda NÃO tem isso.

### Vai precisar:
Biblioteca:
```cpp
#include <HTTPClient.h>
```

### Fluxo:
1. Criar URL da API:
```
https://api.openweathermap.org/data/2.5/weather?q=Criciuma&appid=SUA_KEY&units=metric
```

2. Fazer GET:
```cpp
HTTPClient http;
http.begin(url);
int httpCode = http.GET();
```

3. Ler resposta JSON:
```cpp
String payload = http.getString();
```

---

## 3. 📦 Interpretar JSON

Você precisa extrair:

```json
"main": {
  "temp": 25.3
}
```

### Vai precisar da biblioteca:
```cpp
#include <ArduinoJson.h>
```

### Extração:
```cpp
DynamicJsonDocument doc(1024);
deserializeJson(doc, payload);

float tempExterna = doc["main"]["temp"];
```

---

## 4. ☁️ Enviar DOIS dados ao ThingSpeak

Hoje você envia:
- Temperatura interna
- Umidade
- LDR

### Vai mudar para:
- Campo 1 → temperatura interna
- Campo 2 → temperatura externa

```cpp
ThingSpeak.setField(1, tempInterna);
ThingSpeak.setField(2, tempExterna);
```

---

# 🧩 Estrutura final do sistema

## Loop completo ficará assim:

1. Verifica Wi-Fi  
2. Lê DHT11  
3. Faz requisição HTTP  
4. Extrai temperatura externa  
5. Envia para ThingSpeak  
6. Aguarda 20s  

---

# ⚠️ Pontos importantes (evita dor de cabeça)

### 🔑 API Key do OpenWeather
Você precisa criar em:
- https://openweathermap.org/api

---

### ⏱ Tempo de delay
Você já está correto:
```cpp
delay(20000);
```

---

### 📶 Internet instável
Adicione verificação:
```cpp
if (httpCode > 0)
```

---

# 🗺️ Plano de implementação (ordem ideal)

### Etapa 1
✅ Trocar Wi-Fi para rede simples  

### Etapa 2
✅ Testar DHT11 sozinho  

### Etapa 3
✅ Fazer requisição HTTP (printar JSON no Serial)  

### Etapa 4
✅ Extrair temperatura externa  

### Etapa 5
✅ Integrar com ThingSpeak  

---

# 🚀 Se quiser ir mais rápido

Posso te entregar o código completo já pronto com:
- Wi-Fi simples
- OpenWeather funcionando
- JSON tratado
- ThingSpeak integrado

Ou podemos fazer passo a passo (melhor pra aprender).

👉 Qual você prefere:
1. Código completo direto  
2. Implementação guiada passo a passo