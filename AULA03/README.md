# Aula 03: Integração com APIs e Publicação em Nuvem

## Objetivos da Aula

Nesta aula você irá:

- Compreender as requisições HTTP (GET e POST)
- Interagir com API's via navegador
- Interpretar respostas em formato JSON
- Integrar sensores físicos com serviços web
- Publicar dados em nuvem

---

## Conceitos Importantes

### Requisição HTTP GET
Utilizada para **obter dados** de um servidor.

Exemplo:
```
https://api.openweathermap.org/data/2.5/weather?q=Criciuma&appid=SUA_API_KEY&units=metric
```

---

### JSON (JavaScript Object Notation)

Formato utilizado para troca de dados entre sistemas.

Exemplo de resposta:
```json
{
  "main": {
    "temp": 25.3
  }
}
```

---

## Desafio Prático

### Parte 1: Conexão Wi-Fi

Conecte o ESP32 na rede:

- **SSID:** Turma IOT  
- **Senha:** 12345678  

---

### Parte 2: Leitura de Sensor

- Utilize o sensor **DHT11**
- Leia a temperatura ambiente

---

### Parte 3: Integração com API

- Busque a temperatura externa em Criciúma via API (OpenWeather)

---

### Parte 4: Envio para Nuvem

- Envie a temperatura interna (DHT11) e externa (API) para o ThingSpeak