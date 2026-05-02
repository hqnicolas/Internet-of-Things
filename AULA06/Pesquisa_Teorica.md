# Pesquisa Teórica - HTTP, MQTT e CoAP em IoT

## 1. Análise Técnica

### HTTP

O **HTTP** é um protocolo de aplicação baseado principalmente no modelo **requisição/resposta**. Em aplicações web tradicionais, um cliente envia uma requisição para um servidor e recebe uma resposta com o conteúdo solicitado. No contexto de IoT, isso costuma ser usado quando um dispositivo precisa consultar ou enviar dados para uma API web, como plataformas em nuvem, dashboards e serviços REST.

O HTTP define a semântica de métodos como `GET`, `POST`, `PUT` e `DELETE`. Em projetos IoT educacionais e em muitos sistemas embarcados simples, o uso mais comum ainda é **HTTP/1.1 sobre TCP**. Vale observar que, tecnicamente, versões modernas como **HTTP/3** usam QUIC sobre UDP; portanto, quando comparamos HTTP com MQTT e CoAP nesta atividade, estamos falando do uso mais comum em IoT introdutório: **HTTP tradicional com maior overhead e conexão confiável**. Isso é uma inferência baseada no contexto do enunciado e no histórico das aulas com APIs e ThingSpeak.  
Fonte: [RFC 9110](https://www.rfc-editor.org/rfc/rfc9110), [RFC 9112](https://www.rfc-editor.org/rfc/rfc9112.html).

**Camada de transporte usada:** normalmente **TCP**.  
**Modelo de comunicação:** cliente-servidor, requisição/resposta.  
**Cenários de uso:**

- envio de dados para APIs REST
- integração com plataformas web
- comunicação com serviços em nuvem
- dispositivos que precisam conversar diretamente com sistemas web já existentes

### MQTT

O **MQTT** é um protocolo de mensagens leve baseado no modelo **publish/subscribe**. Em vez de cada dispositivo falar diretamente com todos os outros, existe um **broker** intermediando a comunicação. Um dispositivo publica mensagens em um tópico, e os clientes inscritos nesse tópico recebem os dados.

Segundo a especificação oficial do OASIS, o MQTT foi projetado para ser leve, simples de implementar e adequado a ambientes restritos, incluindo cenários de **M2M** e **IoT**. Ele opera sobre **TCP/IP** ou outros transportes que ofereçam conexão **ordenada, bidirecional e sem perda**.

Um ponto forte do MQTT é o suporte a **três níveis de QoS**:

- **QoS 0:** entrega no melhor esforço, podendo haver perda
- **QoS 1:** entrega garantida, mas com possibilidade de duplicatas
- **QoS 2:** entrega exatamente uma vez

Isso torna o MQTT especialmente útil quando a confiabilidade da entrega precisa ser ajustada de acordo com o tipo de dado transmitido.

**Camada de transporte usada:** **TCP**.  
**Modelo de comunicação:** publish/subscribe com broker.  
**Cenários de uso:**

- telemetria periódica de sensores
- integração com brokers locais ou em nuvem
- monitoramento remoto
- automação residencial e industrial
- sistemas com múltiplos consumidores dos mesmos dados

Fonte: [MQTT Version 5.0 - OASIS](https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html).

### CoAP

O **CoAP** (*Constrained Application Protocol*) é um protocolo de aplicação criado especificamente para dispositivos e redes restritas. Ele segue o modelo **requisição/resposta**, com conceitos parecidos com os da Web, como **URIs**, recursos e métodos, mas com formato muito mais enxuto.

De acordo com o **RFC 7252**, o CoAP foi projetado para nós restritos e redes com baixo throughput, alta taxa de erro e baixo consumo de energia. Ele usa **UDP entre endpoints** e um **cabeçalho binário fixo de 4 bytes**, o que reduz o overhead. Também possui um mecanismo próprio de confiabilidade opcional: mensagens **Confirmable (CON)** são retransmitidas até que o destinatário envie um **ACK**.

Isso significa que, embora use UDP, o CoAP não é simplesmente “sem confiabilidade”; ele implementa confirmações no próprio protocolo quando necessário.

**Camada de transporte usada:** **UDP**.  
**Modelo de comunicação:** requisição/resposta orientada a recursos.  
**Cenários de uso:**

- sensores e atuadores em rede local
- automação predial
- smart energy
- dispositivos com pouca memória e processamento
- aplicações que precisam de baixa sobrecarga de rede

Fonte: [RFC 7252 - CoAP](https://www.rfc-editor.org/rfc/rfc7252), [RFC Info 7252](https://www.rfc-editor.org/info/rfc7252).

---

## 2. Quadro Comparativo: HTTP vs MQTT vs CoAP

| Critério | HTTP | MQTT | CoAP |
| :--- | :--- | :--- | :--- |
| **Modelo de comunicação** | Requisição/resposta | Publish/Subscribe | Requisição/resposta |
| **Transporte** | Normalmente TCP no uso clássico de IoT | TCP | UDP |
| **Confiabilidade** | Herdada do TCP; resposta por requisição | QoS 0, 1 e 2 | Confirmable/ACK no próprio protocolo quando necessário |
| **Overhead** | Maior | Baixo | Muito baixo |
| **Consumo de energia** | Maior, devido a cabeçalhos e sessões mais pesadas | Menor que HTTP | Geralmente menor que HTTP e muito adequado a dispositivos restritos |
| **Simplicidade de implementação** | Muito simples quando já existe API web | Simples, mas exige broker | Simples para recursos leves, mas menos comum que HTTP |
| **Escalabilidade** | Boa para integração web, menos eficiente para muitos dispositivos publicando com frequência | Muito boa para muitos produtores/consumidores | Boa em redes locais e sistemas embarcados distribuídos |
| **Formato típico de uso** | REST, APIs, nuvem | Tópicos e mensagens | Recursos como `/sensor`, `/led`, `/status` |
| **Exemplos de uso real** | APIs web, ThingSpeak, dashboards, serviços REST | Telemetria IoT, automação, brokers em nuvem | Automação predial, smart energy, sensores/atuadores restritos |

### Observações importantes

- O **HTTP** é excelente para integração com serviços web existentes, mas tende a ser mais “pesado” para dispositivos pequenos.
- O **MQTT** é muito eficiente quando vários sistemas precisam consumir os mesmos dados.
- O **CoAP** é muito interessante quando o dispositivo precisa expor recursos leves, como se fosse um “mini servidor” para sensores e atuadores.

---

## 3. Vantagens Técnicas dos Protocolos Leves em Relação ao HTTP Tradicional

Tanto o **MQTT** quanto o **CoAP** foram projetados para reduzir limitações do HTTP tradicional em cenários de IoT:

- menor overhead de mensagens
- menor consumo de banda
- menor consumo de energia
- melhor adaptação a dispositivos com pouca memória e processamento
- melhor desempenho em redes com restrições

No MQTT, essa vantagem aparece principalmente no transporte leve de mensagens e no desacoplamento promovido pelo broker. No CoAP, aparece no uso de UDP, no cabeçalho binário enxuto e no foco em dispositivos restritos.

---

## 4. Conclusão Crítica

### Em quais tipos de projeto IoT é mais vantajoso usar MQTT?

O **MQTT** é mais vantajoso quando o projeto precisa de **troca contínua de mensagens**, especialmente em cenários com **muitos dispositivos** ou **vários sistemas consumidores** dos mesmos dados. Ele é uma ótima escolha para:

- telemetria periódica
- monitoramento remoto em tempo real
- integração com plataformas em nuvem
- automação com vários assinantes
- aplicações em que o dado precisa ser redistribuído para dashboards, bancos de dados e alertas ao mesmo tempo

Nesses casos, o modelo **publish/subscribe** reduz o acoplamento entre produtores e consumidores. Além disso, os níveis de **QoS** permitem ajustar a confiabilidade conforme a criticidade da informação.

### Em quais tipos de projeto IoT é mais vantajoso usar CoAP?

O **CoAP** é mais vantajoso quando o projeto precisa de **comunicação leve, direta e orientada a recursos**, principalmente em redes locais e dispositivos com recursos bem limitados. Ele é uma boa escolha para:

- leitura sob demanda de sensores
- controle de atuadores simples
- automação predial
- dispositivos com pouca memória, pouca bateria ou rede restrita
- cenários em que um nó precisa expor recursos como `/ldr`, `/led` e `/status`

Em outras palavras, o CoAP funciona muito bem quando o dispositivo precisa agir como um pequeno servidor de recursos, com menor overhead que o HTTP tradicional.

### Síntese final

Se o objetivo principal for **publicar eventos e telemetria para múltiplos interessados**, o **MQTT** tende a ser a melhor opção.  
Se o objetivo principal for **acessar ou controlar recursos diretamente em dispositivos restritos**, o **CoAP** tende a ser mais vantajoso.

Assim, os dois protocolos são importantes em IoT, mas resolvem problemas diferentes:

- **MQTT** favorece distribuição de mensagens
- **CoAP** favorece acesso leve a recursos

---

## 5. Referências

1. [RFC 9110 - HTTP Semantics](https://www.rfc-editor.org/rfc/rfc9110)
2. [RFC 9112 - HTTP/1.1](https://www.rfc-editor.org/rfc/rfc9112.html)
3. [MQTT Version 5.0 - OASIS Standard](https://docs.oasis-open.org/mqtt/mqtt/v5.0/os/mqtt-v5.0-os.html)
4. [RFC 7252 - The Constrained Application Protocol (CoAP)](https://www.rfc-editor.org/rfc/rfc7252)
5. [RFC Info 7252 - CoAP Summary](https://www.rfc-editor.org/info/rfc7252)
