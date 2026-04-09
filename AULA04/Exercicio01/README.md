# Exercicio 1 - ESP32 WebServer Hello World

## Objetivo
Criar um webserver sincronico no ESP32 usando a biblioteca `WebServer.h`.

Neste exercicio, o ESP32 deve funcionar como um servidor web simples e responder com uma pagina HTML basica ao acessar a rota principal `/`.

## O que o programador deve fazer
1. Configurar o projeto para usar as bibliotecas `WiFi.h` e `WebServer.h`.
2. Colocar o ESP32 em modo `WIFI_AP` ou `WIFI_STA`, conforme orientacao do professor.
3. Criar um servidor HTTP na porta `80`.
4. Implementar uma funcao para atender a rota `/`.
5. Fazer essa rota retornar uma pagina HTML com uma mensagem de boas-vindas, por exemplo:
   - titulo da pagina
   - nome da disciplina
   - texto "Hello World" ou "Servidor Web no ESP32"
6. Iniciar o servidor no `setup()`.
7. Chamar `server.handleClient()` continuamente no `loop()`.
8. Exibir no monitor serial o endereco IP usado para acessar a pagina.

## Requisitos minimos
- Usar `WebServer server(80);`
- Ter uma rota `/` funcionando
- Retornar conteudo HTML com status `200`
- Permitir acesso pelo navegador

## Exemplo de comportamento esperado
- O ESP32 sobe o servidor
- O monitor serial mostra o IP
- Ao abrir o navegador no IP do ESP32, a pagina exibe uma mensagem simples de teste

## Sugestao de estrutura
```cpp
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Hello World</h1>");
}

void setup() {
  // configurar WiFi
  // registrar rota
  // iniciar servidor
}

void loop() {
  server.handleClient();
}
```

## Checklist
- O codigo compila sem erros
- O ESP32 conecta ou cria a rede corretamente
- O IP aparece no serial monitor
- A rota `/` responde no navegador
- A pagina HTML e carregada com sucesso
