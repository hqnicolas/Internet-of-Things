# Exercicio 4 - SPIFFS Web Server no ESP32

## Objetivo
Criar um webserver no ESP32 utilizando `SPIFFS` para armazenar e servir os arquivos da interface web, como `HTML`, `CSS` e `JavaScript`.

Neste exercicio, o aluno deve separar a interface web do codigo principal do microcontrolador, deixando a pagina armazenada no sistema de arquivos interno do ESP32.

## O que o programador deve fazer
1. Configurar o projeto para usar:
   - `WiFi.h`
   - `WebServer.h`
   - `SPIFFS.h`
2. Inicializar o `SPIFFS` no `setup()`.
3. Criar uma pasta de dados com os arquivos da interface web, por exemplo:
   - `index.html`
   - `style.css`
   - `script.js`
4. Fazer o upload desses arquivos para o sistema de arquivos do ESP32.
5. Configurar o servidor para responder a rota `/` carregando o arquivo `index.html` salvo no `SPIFFS`.
6. Servir tambem os arquivos estaticos de estilo e script, se forem utilizados.
7. Manter o atendimento das requisicoes com `server.handleClient()` no `loop()`.

## Requisitos minimos
- O `SPIFFS` deve ser iniciado com sucesso
- A pagina principal deve ser lida a partir da memoria flash
- O navegador deve carregar a interface sem que o HTML fique embutido diretamente no codigo C++
- O servidor deve responder corretamente na porta `80`

## Conceito principal
Nos exercicios anteriores, a pagina HTML podia ser escrita dentro de uma `String` no codigo.

Neste exercicio, a proposta e melhorar a organizacao:
- os arquivos da pagina ficam armazenados no `SPIFFS`
- o codigo do ESP32 fica responsavel por servir esses arquivos
- a interface se torna mais facil de manter e expandir

## Exemplo de estrutura esperada
```text
Exercicio04/
|-- README.md
|-- data/
|   |-- index.html
|   |-- style.css
|   `-- script.js
`-- src ou sketch principal
```

## Exemplo de implementacao
```cpp
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>

WebServer server(80);

void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    server.send(500, "text/plain", "Erro ao abrir index.html");
    return;
  }

  server.streamFile(file, "text/html");
  file.close();
}

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("Erro ao iniciar SPIFFS");
    return;
  }

  // configurar WiFi
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
```

## Sugestoes de extensao
O aluno pode reutilizar este exercicio para:
- mover a interface do controle de LEDs para o `SPIFFS`
- mover a pagina de leitura dos sensores para o `SPIFFS`
- criar uma interface mais bonita com `CSS`
- usar `JavaScript` para buscar dados em rotas como `/sensores`

## Exemplo de comportamento esperado
- O ESP32 inicia o `SPIFFS`
- O monitor serial confirma que o sistema de arquivos foi montado
- O servidor inicia normalmente
- Ao acessar o IP do ESP32 no navegador, a pagina `index.html` carregada do `SPIFFS` e exibida
- Se houver `CSS` e `JavaScript`, eles tambem sao carregados corretamente

## Checklist
- O `SPIFFS.begin()` funciona sem erro
- Os arquivos foram enviados para a flash do ESP32
- A rota `/` carrega `index.html`
- O HTML nao esta hardcoded no firmware
- A pagina abre corretamente no navegador
- Arquivos estaticos adicionais funcionam corretamente
