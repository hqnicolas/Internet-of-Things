# Lista de Exercícios – Internet das Coisas

---

**Conteúdo:** Divisor de Tensão, Sensores Resistivos (LDR) e Resolução de ADC

**1)** Em sistemas de Internet das Coisas (IoT), a aquisição de dados do ambiente físico frequentemente exige o uso de sensores analógicos resistivos, como o LDR (Light Dependent Resistor), configurados em circuitos de condicionamento de sinal do tipo divisor de tensão. Para a digitalização desses sinais, microcontroladores modernos como o ESP32 possuem conversores analógico-digitais (ADC) integrados com resolução configurável de até 12 bits, operando em uma faixa de tensão de 0 a 3,3 V.

Considere um cenário de laboratório em que um LDR está conectado em série com um resistor fixo de R1 = 10 kΩ alimentado por VCC = 3,3 V, formando um divisor de tensão onde a saída analógica (AO) é medida diretamente sobre o LDR (conectado ao GND). Sabe-se que o LDR apresenta uma resistência de 5 kΩ sob luz ambiente intensa e 50 kΩ quando totalmente coberto (sombra).

A partir da análise do circuito de condicionamento de sinal apresentado e do funcionamento do ADC de 12 bits do ESP32, avalie as afirmações a seguir:

* **I.** Sob luz ambiente intensa, a tensão gerada na saída analógica (AO) será de 1,1 V, fazendo com que a função `analogRead()` do ESP32 retorne o valor digital aproximado de 1365.
* **II.** Em condições de sombra, a resistência do LDR se eleva, deslocando a tensão no pino (AO) para 2,75 V, o que resulta em uma leitura digital de aproximadamente 3412 no código do dispositivo de borda.
* **III.** Caso a posição física do LDR e do resistor de R1 = 10 kΩ fosse invertida no divisor de tensão (LDR conectado ao VCC e R1 ao GND), o valor digital retornado pelo ADC passaria a aumentar proporcionalmente com o aumento da intensidade luminosa.

É correto o que se afirma em:
**A.** I, apenas.
**B.** III, apenas.
**C.** I e II, apenas.
**D.** II e III, apenas.
**E.** I, II e III.

---

**Conteúdo:** Comparadores Analógicos por Hardware (LM393) e Limiar de Atuadores

**2)** Módulos comerciais de sensores para IoT frequentemente incorporam o circuito integrado LM393, um comparador de tensão analógico duplo. Este componente permite gerar uma saída digital direta (DO) chaveada por hardware, mitigando a necessidade de processamento contínuo do ADC para tarefas de interrupção ou automação local simples.

Em um circuito padrão alimentado a 3,3 V com um resistor fixo de 10 kΩ, a tensão do divisor (AO) é conectada à entrada não-inversora (pino 3) do LM393, enquanto a entrada inversora (pino 2) recebe uma tensão de referência (Vref) ajustável por meio de um trimpot.

A respeito do funcionamento dinâmico desse hardware de condicionamento, considere a seguinte situação prática: o projetista deseja que o pino digital DO mude de estado lógico exatamente quando a resistência do LDR atingir o limiar crítico de 8 kΩ. Para isso, ele calibra o trimpot para fixar a tensão de referência em Vref = 1,467 V. Posteriormente, durante a operação, uma variação de luz eleva a condutividade do sensor, fazendo com que a tensão analógica instantânea em AO caia para 1,2 V.

Com base nessa situação, analise as asserções a seguir e a relação proposta entre elas:

* **I.** Sob a tensão analógica instantânea de 1,2 V no pino AO, a saída digital DO do circuito integrado assumirá nível lógico BAIXO (0 V).

**PORQUE**

* **II.** De acordo com as leis que regem os comparadores analógicos baseados em amplificadores operacionais em malha aberta, sempre que a tensão aplicada na entrada não-inversora (V+) for estritamente menor do que a tensão na entrada inversora (V-), a saída do dispositivo será saturada no potencial de alimentação inferior, que neste circuito equivale ao referencial de terra (GND).

A respeito dessas asserções, assinale a opção correta:
**A.** As asserções I e II são proposições verdadeiras, e a II é uma justificativa correta da I.
**B.** As asserções I e II são proposições verdadeiras, mas a II não é uma justificativa correta da I.
**C.** A asserção I é uma proposição verdadeira, e a II é uma proposição falsa.
**D.** A asserção I é uma proposição falsa, e a II é uma proposição verdadeira.
**E.** As asserções I e II são proposições falsas.

---

**Conteúdo:** Não-linearidade de ADC, Ruído de Radiofrequência (WiFi) e Técnicas de Mitigação

**3)** O projeto de nós de sensores robustos voltados para aplicações reais de IoT (como estufas agrícolas ou monitoramento ambiental industrial) exige que o engenheiro compreenda os limites físicos das plataformas de desenvolvimento. O microcontrolador ESP32 possui um conversor analógico-digital (ADC) de aproximações sucessivas (SAR) que apresenta uma conhecida não-linearidade nas extremidades de sua curva de resposta, manifestando zonas mortas (dead zones) próximas a 0 V e acima de 3,2 V.

Adicionalmente, quando o modem de radiofrequência do chip está operando ativamente na transmissão de pacotes via WiFi (seja em modo Station ou AP), surtos de ruído de alta frequência são induzidos nas linhas internas de alimentação do chip, refletindo diretamente em flutuações e instabilidades nas leituras analógicas brutas.

Imagine que sua equipe de engenharia precisa ler com precisão um sensor industrial de umidade do solo cuja saída analógica varia linearmente de 0,2 V (solo totalmente seco) a 3,1 V (solo totalmente saturado).

Considerando os problemas de não-linearidade e ruído por chaveamento de RF do ESP32, responda aos itens a seguir:

**a)** Explique o impacto prático que a curva de não-linearidade do ADC do ESP32 trará para as métricas da estufa especificamente nas proximidades do limite máximo de umidade do sensor 3,1 V.
**b)** Proponha uma solução a nível de hardware (circuito) e uma solução a nível de firmware (software embarcado) para garantir a estabilidade e a fidelidade das leituras desse sensor frente ao ruído gerado pelas transmissões WiFi do microcontrolador.

---

**Conteúdo:** Arquitetura de Servidores Síncronos vs. Assíncronos e Modos WiFi

**4)** Na engenharia de sistemas embarcados voltados para IoT, a escolha da arquitetura do software servidor desempenha um papel crítico na capacidade de resposta do dispositivo de borda. O ecossistema do ESP32 permite a implementação de servidores baseados na biblioteca síncrona tradicional (`WebServer.h`) ou na biblioteca assíncrona (`AsyncWebServer.h`). A eficiência dessas abordagens está diretamente ligada à forma como o loop principal do firmware gerencia o tempo de processamento e a escuta por requisições na pilha de protocolos de rede, além do modo de operação do modem sem fio (`WIFI_AP` ou `WIFI_STA`).

Considere um cenário de aplicação real em que um ESP32 atua como um nó concentrador de telemetria. Avalie as seguintes afirmações a respeito do comportamento desse sistema:

* **I.** Em aplicações baseadas em `WebServer.h`, a execução do método `server.handleClient()` no `loop()` principal é estritamente obrigatória para processar as requisições entrantes. Caso o fluxo de execução do `loop()` seja bloqueado por uma rotina demorada de leitura de sensores, o servidor ficará temporariamente incapaz de responder aos clientes, podendo gerar falhas de timeout.
* **II.** A arquitetura baseada em `AsyncWebServer.h` opera em segundo plano tirando proveito dos recursos de multitarefa e callbacks da pilha de rede do sistema operacional. Desse modo, o servidor consegue registrar e responder a múltiplas conexões TCP simultâneas sem a necessidade de manter uma função de varredura (polling) ativa no `loop()` principal do firmware.
* **III.** Quando o dispositivo é configurado utilizando a diretiva `WiFi.mode(WIFI_AP)`, ele assume o papel de ponto de acesso, gerando sua própria sub-rede local e atribuindo um endereço IP padrão (como 192.168.4.1) aos dispositivos clientes que a ele se conectarem. Essa configuração permite que o ESP32 sirva a interface web de controle localmente, independentemente de uma infraestrutura de rede externa de roteamento.

É correto o que se afirma em:
**A.** I, apenas.
**B.** II, apenas.
**C.** I e II, apenas.
**D.** II e III, apenas.
**E.** I, II e III.

---

**Conteúdo:** Métodos HTTP (GET e POST) e Estruturas de Dados JSON

**5)** Na especificação de APIs REST para comunicação entre interfaces de usuário e dispositivos de borda IoT, os métodos HTTP devem seguir regras semânticas bem definidas para garantir a integridade do sistema. Para o tráfego dessas mensagens, o formato JSON (JavaScript Object Notation) consolidou-se como o padrão industrial devido à sua leveza, estrutura legível e facilidade de empacotamento em sistemas distribuídos.

A respeito do uso prático desses padrões no controle de atuadores em plataformas embarcadas, analise as asserções a seguir e a relação proposta entre elas:

* **I.** Para alterar o estado de um atuador no ESP32, como acionar um relé ou mudar a cor de um LED RGB, o projetista deve implementar no manipulador (handler) do servidor a recepção de uma requisição do tipo HTTP POST, evitando o uso do método HTTP GET para essa finalidade.

**PORQUE**

* **II.** De acordo com as diretrizes do protocolo HTTP, o método GET possui semântica estritamente nula quanto a efeitos colaterais (safe method), devendo ser utilizado exclusivamente para a leitura de dados (como obter a telemetria atual de um sensor) sem que sua execução modifique o estado interno das variáveis ou dos pinos físicos do hardware do servidor.

A respeito dessas asserções, assinale a opção correta:
**A.** As asserções I e II são proposições verdadeiras, e a II é uma justificativa correta da I.
**B.** As asserções I e II são proposições verdadeiras, mas a II não é uma justificativa correta da I.
**C.** A asserção I é uma proposição verdadeira, e a II é uma proposição falsa.
**D.** A asserção I é uma proposição falsa, e a II é uma proposição verdadeira.
**E.** As asserções I e II são proposições falsas.

---

**Conteúdo:** Refatoração de Código, Temporização Não-Bloqueante (millis()) e Concorrência

**6)** Um engenheiro de computação herdou o código legado de uma estação meteorológica desenvolvida sobre a biblioteca síncrona `WebServer.h`. No firmware original, a leitura de um sensor de temperatura e umidade DHT11 era realizada dentro do `loop()` principal utilizando a instrução `delay(2000)` para respeitar o tempo de estabilização do sensor estipulado pelo fabricante. Logo após o atraso, a função de escuta do servidor era chamada, conforme ilustrado abaixo:

```cpp
void loop() {
 // Código omitido
}

void loop() {
 int temperatura = dht.readTemperature();
 int umidade = dht.readHumidity();

 delay(2000); // Aguarda estabilização do sensor
 server.handleClient(); // Processa requisições HTTP síncronas
}

```

Ao colocar o dispositivo em campo em uma rede corporativa com múltiplos dashboards web realizando consultas simultâneas na API, o sistema passou a apresentar quedas constantes de conexão TCP, travamentos na interface do usuário e falhas críticas de indisponibilidade (timeout).

Com o objetivo de resolver em definitivo os travamentos, sua equipe recebeu a tarefa de migrar a arquitetura de software da estação meteorológica para a biblioteca assíncrona `AsyncWebServer.h` e eliminar qualquer temporização bloqueante do código de controle.

Com base nos conceitos de concorrência embarcada e protocolos web, responda aos itens a seguir:

**a)** Explique, do ponto de vista do fluxo de execução do processador e do tempo de ciclo do `loop()`, por que a combinação da função `delay(2000)` com o servidor síncrono causa a perda de requisições quando múltiplos clientes acessam a estação ao mesmo tempo.
**b)** Apresente um exemplo de reestruturação lógica em linguagem C++ (padrão Arduino/ESP32) para a função `loop()`, demonstrando como realizar a leitura periódica do sensor a cada 2 segundos utilizando a função `millis()` de forma não-bloqueante. Justifique como essa nova abordagem permite que o AsyncWebServer processe as requisições em segundo plano com alta performance.

---

**Conteúdo:** Concorrência, Multitarefa (FreeRTOS Tasks), Filas (Queues) e Software Timers

**7)** Em projetos de IoT industrial de alta performance desenvolvidos para o microcontrolador ESP32, o uso isolado de loops de varredura se mostra ineficiente para gerenciar múltiplos subsistemas concorrentes. Para segmentar de forma robusta as funções de coleta de dados de sensores, atualização de displays locais, barramentos de comunicação MQTT e servidores web assíncronos, engenheiros de computação utilizam o kernel do FreeRTOS integrado ao ambiente de desenvolvimento. A correta aplicação de primitivas como Tarefas (Tasks), Filas (Queues) e Temporizadores de Software (Software Timers) é indispensável para garantir o determinismo temporal e evitar a corrupção de memória em sistemas de tempo real.

Considere um nó de borda IoT estruturado com FreeRTOS em que a aquisição de sensores ocorre periodicamente, os dados são enviados para um broker MQTT corporativo e uma API JSON é servida de forma assíncrona via HTTP. Analise as seguintes afirmações sobre a arquitetura desse software embarcado:

* **I.** A criação de Tasks independentes para o isolamento de responsabilidades (uma para leitura física de sensores e outra para gerenciamento da pilha MQTT) evita que latências ou falhas temporárias de conexão na rede degradem a precisão da temporização da coleta de dados do processo. Para transferir de forma segura (thread-safe) os dados coletados entre essas tarefas distintas, eliminando o risco de sobreescrita de memória, devem ser utilizadas as Filas (Queues) do FreeRTOS.
* **II.** Os manipuladores de rotas (handlers) da biblioteca `AsyncWebServer.h` operam de forma orientada a eventos por meio de callbacks executados em segundo plano pela pilha de rede. Se uma tarefa do FreeRTOS atualizar continuamente as variáveis de uma estrutura global de telemetria (`struct sensor_data`) no exato momento em que um callback assíncrono lê essa mesma estrutura para gerar uma resposta JSON, ocorrerá uma condição de corrida (race condition), tornando obrigatório o uso de um Mutex para garantir a exclusão mútua.
* **III.** Os Temporizadores de Software (Software Timers) do FreeRTOS permitem agendar a execução de funções de callback em momentos específicos sem a necessidade de criar uma tarefa dedicada para controle de tempo. Por rodarem no contexto de uma única tarefa de serviço do próprio kernel (Daemon Task), as funções de callback desses timers não devem conter nenhuma instrução bloqueante de longa duração (como buscas de rede TCP ou atrasos explícitos), sob o risco de atrasar a execução de todos os outros timers do sistema.

É correto o que se afirma em:
**A.** I, apenas.
**B.** III, apenas.
**C.** I e II, apenas.
**D.** II e III, apenas.
**E.** I, II e III.

---

**Conteúdo:** Arquitetura Dual-Core do ESP32, Mecanismos de Sincronização (Mutex) e Condição de Corrida

**8)** O SoC (System on a Chip) ESP32 destaca-se no cenário de IoT por integrar um processador Xtensa dual-core de 32 bits (Core 0 e Core 1). Por padrão, a pilha de protocolos de comunicação sem fio (WiFi e Bluetooth) e o motor de eventos assíncronos da biblioteca `AsyncWebServer.h` são executados de forma transparente no Core 0. Ao mesmo tempo, as rotinas de controle desenvolvidas pelo programador no ecossistema Arduino/FreeRTOS, incluindo o processamento do `loop()` principal e a execução de novas tarefas criadas via rotina `xTaskCreate()`, rodam nativamente no Core 1.

Considerando o comportamento de um firmware de um gateway IoT que faz uso simultâneo de processamento multitarefa e servidores de rede assíncronos, analise as asserções a seguir e a relação proposta entre elas:

* **I.** Se o projetista instanciar uma tarefa no FreeRTOS para rodar continuamente no Core 1 manipulando variáveis globais de estado de atuadores, e configurar rotas na API do AsyncWebServer rodando no Core 0 para ler e alterar essas mesmas variáveis, ele será obrigado a proteger esse bloco de memória compartilhada utilizando primitivas de sincronização, como Mutexes.

**PORQUE**

* **II.** Operações simultâneas de leitura e escrita executadas por núcleos de processamento físicos distintos sobre o mesmo endereço de memória RAM configuram um cenário de verdadeiro paralelismo de hardware, o que impossibilita a atomicidade das instruções de software e pode corromper os dados compartilhados caso não haja exclusão mútua ativa.

A respeito dessas asserções, assinale a opção correta:
**A.** As asserções I e II são proposições verdadeiras, e a II é uma justificativa correta da I.
**B.** As asserções I e II são proposições verdadeiras, mas a II não é uma justificativa correta da I.
**C.** A asserção I é uma proposição verdadeira, e a II é uma proposição falsa.
**D.** A asserção I é uma proposição falsa, e a II é uma proposição verdadeira.
**E.** As asserções I e II são proposições falsas.

---

**Conteúdo:** Engenharia de Sistemas, Projeto de Soluções Integradas (FreeRTOS + MQTT + Async WebServer)

**9)** Você foi designado como engenheiro chefe para projetar a arquitetura de firmware de um sistema industrial de monitoramento para uma caldeira a vapor, utilizando um ESP32. O sistema possui restrições severas de operação e deve cumprir obrigatoriamente os seguintes requisitos de engenharia:

* **R1 (Coleta de Dados):** Capturar continuamente os dados de 3 sensores analógicos distintos (Pressão, Temperatura e Vazão) em intervalos determinísticos rígidos de exatos 100 ms.
* **R2 (Interface Local):** Atualizar um display físico de 7 segmentos acoplado ao painel da máquina a cada 250 ms para exibir de forma alternada as leituras dos sensores.
* **R3 (Telemetria Cloud):** Empacotar os dados lidos em formato JSON e publicá-los via protocolo MQTT (modelo publish/subscribe) em um servidor central (Broker) a cada 2 segundos.
* **R4 (API e Supervisório Web):** Disponibilizar um endpoint HTTP assíncrono (`GET /api/status`) para retornar instantaneamente o estado atualizado dos sensores em formato JSON sempre que um operador externo acessar a interface web local do equipamento.

Considerando a integração obrigatória entre os conceitos de sistemas operacionais de tempo real e comunicação assíncrona em IoT, responda aos itens a seguir:

**a)** Esboce o diagrama de blocos arquitetural da solução de software embarcado sugerida para este cenário, detalhando a divisão das tarefas (Tasks) independentes do FreeRTOS, a distribuição de suas responsabilidades e especificando quais primitivas de sincronização e comunicação inter-tarefas (ex: Queues, Mutexes, Semaphores) serão utilizadas para trafegar os dados dos sensores de forma segura entre a captura física e as camadas de rede (MQTT e Servidor Web).
**b)** Justifique tecnicamente por que o uso integrado de Tasks associadas a estruturas de dados protegidas garante que o cumprimento do requisito de tempo real estrito da amostragem analógica (100 ms − R1) jamais seja violado ou atrasado devido a variações de tráfego na rede, quedas de conexão WiFi ou requisições simultâneas na API web assíncrona (`GET /api/status` − R4).

---

**Conteúdo:** Engenharia de Software Embarcado, Alocação Dinâmica de Memória, WebSockets e Otimização de Banda

**10)** A transição de um protótipo funcional de IoT para uma solução comercial em larga escala exige uma análise profunda sobre a gestão de recursos de hardware e rede. Microcontroladores como o ESP32 dispõem de memória RAM limitada (cerca de 520 KB). Práticas comuns de codificação em ambientes de desenvolvimento rápidos, embora funcionais localmente para um único usuário, podem inviabilizar a escalabilidade e a estabilidade do dispositivo de borda sob regimes de alta concorrência ou em períodos prolongados de operação contínua (uptime).

Considere os desafios de desempenho envolvidos no desenvolvimento de uma estação meteorológica embarcada que atende a múltiplos clientes simultâneos em um prédio inteligente. Avalie as seguintes afirmações sobre técnicas de otimização aplicadas a esse sistema:

* **I.** A construção de payloads JSON por meio de concatenação manual de strings primitivas (`String json = "{\"temp\":" + String(t) + ...}`) em rotas de alta frequência gera fragmentação crônica na memória Heap do ESP32 devido à alocação e desalocação dinâmica de buffers. Para mitigar o risco de travamentos por estouro de memória, deve-se adotar o uso de pools de memória estática por meio de bibliotecas como a ArduinoJson.
* **II.** Manter a interface gráfica (HTML/CSS/JavaScript) embutida como raw string (`R"(...)"`) diretamente dentro do código-fonte `.ino` consome espaço crítico na memória RAM estática durante a execução do programa. O design escalonável correto dita que tais arquivos estáticos sejam armazenados na memória Flash não-volátil do chip através de sistemas de arquivos como SPIFFS ou LittleFS, sendo lidos e servidos sob demanda.
* **III.** Configurar a interface do navegador cliente para realizar requisições AJAX periódicas via Fetch API (modelo de polling) a cada 2 segundos degrada consideravelmente a largura de banda e aumenta o consumo de energia do nó de borda. A substituição desse paradigma pelo protocolo WebSockets otimiza a escalabilidade, uma vez que ele substitui as sucessivas aberturas e fechamentos de conexões HTTP por um canal de comunicação bidirecional persistente baseado em um único handshake TCP.

É correto o que se afirma em:
**A.** I, apenas.
**B.** II, apenas.
**C.** I e III, apenas.
**D.** II e III, apenas.
**E.** I, II e III.

---

**Conteúdo:** Segurança em IoT, Autenticação de Dispositivos e Injeção de Parâmetros

**11)** No desenvolvimento de APIs REST expostas por dispositivos embarcados em redes corporativas ou residenciais, a segurança não pode ser tratada como um elemento opcional. A facilidade de interação fornecida por parâmetros expostos em texto plano na URL (como em rotas de controle para atuadores) simplifica o desenvolvimento inicial e os testes de bancada, mas introduz vetores severos de ataque que expõem a integridade física do ambiente monitorado.

A respeito dos riscos associados à exposição de endpoints e técnicas de proteção em sistemas IoT de borda, analise as asserções a seguir e a relação proposta entre elas:

* **I.** Disponibilizar endpoints abertos que controlam diretamente pinos físicos através de parâmetros passados via requisições HTTP do tipo GET (por exemplo, `GET /api/controle/led?pin=23&state=1`) representa uma falha grave de segurança estrutural, sendo indispensável a aplicação de mecanismos de autenticação e a conversão de tipos com validação de limites estritos sobre os parâmetros recebidos.

**PORQUE**

* **II.** O tráfego baseado no protocolo HTTP convencional (porta 80) flui pela rede local em texto claro, o que permite a agentes maliciosos interceptar pacotes via ataques de sniffing (Man-in-the-Middle), além de expor o dispositivo a injeções de comandos caso o valor recebido pela função de conversão (`toInt()`) não seja explicitamente validado contra a tabela de GPIOs permitidos do microcontrolador.

A respeito dessas asserções, assinale a opção correta:
**A.** As asserções I e II são proposições verdadeiras, e a II é uma justificativa correta da I.
**B.** As asserções I e II são proposições verdadeiras, mas a II não é uma justificativa correta da I.
**C.** A asserção I é uma proposição verdadeira, e a II é uma proposição falsa.
**D.** A asserção I é uma proposição falsa, e a II é uma proposição verdadeira.
**E.** As asserções I e II são proposições falsas.

---

**12)** Uma instituição de ensino superior contratou uma equipe de engenharia para implantar um sistema de monitoramento de temperatura em tempo real para as suas 50 salas de aula. O projeto piloto foi desenvolvido instalando um ESP32 em cada sala de aula executando um servidor web assíncrono independente (AsyncWebServer). Para visualizar as temperaturas, o administrador da rede precisa acessar individualmente o endereço IP gerado por cada placa através de um navegador.

Embora o sistema funcione perfeitamente no ambiente de testes controlado do laboratório, a equipe de infraestrutura de TI da universidade barrou a expansão do projeto para as 50 salas. O argumento técnico baseou-se nos severos problemas de escalabilidade, gestão de endereçamento IP na rede corporativa, descoberta de dispositivos e no alto custo de processamento para consolidar todos os dados em uma tela gerencial única para a reitoria.

Com o objetivo de viabilizar o projeto de engenharia, sua equipe deve propor uma reestruturação completa da arquitetura lógica e de rede do sistema, migrando do modelo descentralizado de múltiplos WebServers para um modelo centralizado baseado no protocolo de mensageria MQTT acoplado a uma ferramenta de orquestração de dados (middleware).

Com base nos requisitos arquiteturais de redes IoT e escalabilidade, responda aos itens a seguir:

**a)** Analise dois gargalos técnicos ou limitações da arquitetura original (baseada em 50 servidores web independentes) que justificam a preocupação da equipe de infraestrutura de TI da universidade.
**b)** Desenhe ou descreva textualmente a nova topologia da arquitetura proposta utilizando o protocolo MQTT. Explique como o modelo de comunicação adotado por esse protocolo soluciona os problemas de descoberta de dispositivos, consumo de recursos nas pontas (ESP32) e agregação de dados em um painel gerencial (Dashboard) único.