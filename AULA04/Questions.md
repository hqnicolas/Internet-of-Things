# QUESTÕES DE REFLEXÃO E APRENDIZADO  
**WebServer com ESP32 | Internet das Coisas**

## 📖 INSTRUÇÕES
Responda as questões a seguir com base no conteúdo aprendido na aula prática. Procure ser conciso, mas completo nas respostas. Estas questões ajudam a consolidar o aprendizado e preparar para avaliações futuras.

---

## 🔹 SEÇÃO 1: Conceitos Básicos de WebServer

**Q1.** O que é um WebServer? Explique sua função em um contexto de IoT.  
💡 *Dica:* Pense em como você acessa uma página web. O servidor é o responsável por responder suas requisições.

**Q2.** Qual é a diferença entre `WiFi.mode(WIFI_AP)` e `WiFi.mode(WIFI_STA)`?  
💡 *Dica:* AP = você cria a rede. STA = você se conecta a uma rede existente.

**Q3.** Na Atividade 1, qual é o IP padrão que o ESP32 assume quando atua como AP?  
💡 *Dica:* Observe a mensagem no Serial Monitor. Geralmente é 192.168.x.x

**Q4.** Por que precisamos de `server.handleClient()` no `loop()`? O que aconteceria se não colocássemos?  
💡 *Dica:* `handleClient()` processa requisições chegando. Sem ele, o servidor não responderia.

**Q5.** O que significa HTTP GET e HTTP POST? Quando usamos cada um?  
💡 *Dica:* GET = solicitar dados. POST = enviar dados para o servidor processar.

---

## 🔹 SEÇÃO 2: WebServer Básico (Exemplo 1)

**Q6.** Qual biblioteca você utilizou para criar o webserver na Atividade 1? Há alternativas?  
💡 *Dica:* Usamos `WebServer.h`. Existem outras como `AsyncWebServer` (usada na Atividade 4).

**Q7.** Na função `handleRoot()`, o que significa o parâmetro `200` em `server.send(200, ...)`?  
💡 *Dica:* É um código HTTP. 200 = sucesso, 404 = não encontrado, 500 = erro do servidor.

**Q8.** Por que utilizamos uma raw string (`R"(...)")`) para armazenar HTML no código?  
💡 *Dica:* Permite quebras de linha e caracteres especiais sem precisar de escape (`\n`, `\"`).

**Q9.** Como você testa se a página web está carregando corretamente antes de colocar no ESP32?  
💡 *Dica:* Salve o HTML em um arquivo `.html` local e abra no navegador.

**Q10.** Se você mudar a senha do AP de `"12345678"` para `"senha123"`, o que mais você precisa alterar no código?  
💡 *Dica:* Só o `#define PASSWORD_AP`. Nada mais muda no webserver em si.

---

## 🔹 SEÇÃO 3: Controle de LEDs via Web (Atividade 1)

**Q11.** Qual é a diferença entre POST e GET ao controlar os LEDs? Por que usamos POST em vez de GET?  
💡 *Dica:* GET = solicitar (não modifica estado). POST = enviar dados que modificam estado (ligar/desligar é mudança).

**Q12.** Na função `handleLED()`, como você captura o parâmetro `"led"` que vem da requisição?  
💡 *Dica:* `server.hasArg("led")` verifica se existe. `server.arg("led")` obtém o valor.

**Q13.** Se um LED não acender ao clicar no botão, liste 3 coisas que você verificaria para debugar.  
💡 *Dica:* Verificar:  
1. Pino correto?  
2. `pinMode(pin, OUTPUT)` foi chamado?  
3. `digitalWrite` retornando `HIGH/LOW`?

**Q14.** Por que armazenamos o estado dos LEDs em um array `bool ledState[4]`? Não poderíamos apenas usar `digitalWrite`?  
💡 *Dica:* Array mantém estado em memória. Útil para: confirmar estado, feedback visual, persistência entre requisições.

**Q15.** Como você faria para enviar um LED para um estado que não sabe se é ON ou OFF? Como verificar o estado atual?  
💡 *Dica:* Ler o array `ledState[]` ou usar `digitalRead(LED_PIN)` para ler o pino físico.

---

## 🔹 SEÇÃO 4: Cliente WiFi + Sensores (Atividade 3)

**Q16.** Como você verifica se o ESP32 está conectado à rede WiFi da SATC? Qual a função e o que esperar no Serial Monitor?  
💡 *Dica:* `WiFi.status() == WL_CONNECTED`. `Serial.println()` mostra "WiFi connected!" e o IP atribuído.

**Q17.** Por que lemos sensores dentro da função `handleSensors()` em vez de no `loop()`? Qual a vantagem?  
💡 *Dica:* `loop()` lê continuamente e consome recursos. `handleSensors()` lê apenas quando a requisição chega.

**Q18.** Qual função você usa para ler um valor analógico do LDR? Qual a faixa de valores esperada?  
💡 *Dica:* `analogRead(pin)`. Faixa: 0–4095 (ADC do ESP32 é 12-bit).

**Q19.** Na Atividade 3, usamos `map(analogRead(LDR_PIN), 0, 4095, 0, 100)`. Explique por que normalizamos para 0–100%.  
💡 *Dica:* Valores 0–4095 não são intuitivos. 0–100% é mais legível e independe da resolução do ADC.

**Q20.** JSON é usado para retornar dados dos sensores. Por que não retornamos um simples texto como `"25.5 60 45"`?  
💡 *Dica:* JSON é estruturado, autoexplicativo, fácil de parsear em JavaScript e padrão em IoT/APIs.

---

## 🔹 SEÇÃO 5: Integração e Aplicações Práticas

**Q21.** Descreva um cenário real de IoT onde você usaria um WebServer em um ESP32. Por quê?  
💡 *Dica:* Exemplos: sistema de iluminação inteligente, monitor de temperatura, automação residencial, etc.

**Q22.** Se você tivesse 100 dispositivos ESP32 em um prédio, todos rodando WebServers, qual seria um desafio?  
💡 *Dica:* Endereços IP (DHCP), descoberta de dispositivos, gerenciamento centralizado, segurança.

**Q23.** Como você agregaria dados de múltiplos ESP32s em um único dashboard web? Qual seria a arquitetura?  
💡 *Dica:* ESP32s → servidor central (PC/Cloud) → coleta `/sensors` → dashboard HTML único.

**Q24.** Qual é a diferença entre HTTP e HTTPS? Como implementaria segurança em um WebServer IoT?  
💡 *Dica:* HTTP: sem criptografia. HTTPS: criptografado com SSL/TLS. ESP32 suporta HTTPS, mas requer certificados.

**Q25.** Como você conectaria um banco de dados remoto (MySQL, Firebase) a um ESP32 que serve um WebServer?  
💡 *Dica:* ESP32 faz requisições HTTP/HTTPS para uma API remota. A API acessa o banco de dados e retorna os dados.
