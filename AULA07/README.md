# Aula 07 - Malha de Controle Industrial com MQTT

## Visao Geral

Esta aula teve como foco a implementacao de um sistema de controle industrial distribuido usando **ESP32 + MQTT**.

A proposta foi simular uma fabrica dividida em setores, onde cada grupo controla um conjunto de LEDs por meio de topicos MQTT, permitindo:

- controle individual
- controle setorial
- controle global

Além do controle remoto via broker, a solucao desenvolvida nesta aula tambem incluiu o envio de comandos por botoes locais.

---

## Objetivos da Aula

- compreender o modelo **publish/subscribe** do protocolo MQTT
- conectar o ESP32 a uma rede **Wi-Fi Enterprise**
- usar um broker MQTT publico para troca de mensagens
- controlar atuadores a partir de topicos
- separar os papeis de **receptor** e **emissor**
- aplicar multiplexacao de GPIO quando o mesmo pino precisa atuar como entrada e saida

---

## Estrutura da Pasta

```text
AULA07/
└── Atividade/
    ├── Atividade.ino
    ├── Emissor.ino
    └── README.md
```

Descricao dos arquivos:

- `Atividade/Atividade.ino`: receptor MQTT do grupo, responsavel por assinar topicos e acionar os LEDs
- `Atividade/Emissor.ino`: transmissor MQTT com botoes locais e multiplexacao de GPIO
- `Atividade/README.md`: documentacao detalhada da implementacao pratica

---

## Arquitetura da Solucao

### Receptor

O sketch `Atividade.ino` foi usado como receptor do grupo.

Funcoes principais:

- conectar ao Wi-Fi Enterprise
- conectar ao broker MQTT via TLS
- assinar os topicos do grupo
- receber payloads `ON` e `OFF`
- atualizar os LEDs locais

### Emissor

O sketch `Emissor.ino` foi usado como transmissor.

Funcoes principais:

- conectar ao Wi-Fi Enterprise
- detectar botoes nos GPIOs `4`, `0`, `2` e `15`
- publicar comandos MQTT para outros grupos
- manter os LEDs locais funcionando com multiplexacao entre entrada e saida

---

## Broker e Comunicacao

Configuracao usada no ESP32:

- host: `broker.hivemq.com`
- porta MQTT TLS: `8883`

Configuracao usada no cliente web:

- cliente: `https://www.hivemq.com/demos/websocket-client/`
- host: `mqtt-dashboard.com`
- porta WebSocket SSL: `8884`

Observacao:

- o navegador usa **WebSocket**
- o ESP32 usa **MQTT sobre TCP/TLS**
- por isso as configuracoes do cliente web e da placa nao sao iguais

---

## Padrao de Topicos

Padrao adotado:

```text
satc/gX/ledY
satc/gX/all
satc/all/all
```

Exemplos:

- `satc/g7/led1`
- `satc/g7/led2`
- `satc/g7/led3`
- `satc/g7/led4`
- `satc/g7/all`
- `satc/all/all`

---

## Grupo Implementado

Na versao desenvolvida nesta pasta, o grupo configurado foi o:

```cpp
GROUP_ID = 7
```

Isso significa que:

- o receptor responde aos topicos do `G7`
- o emissor publica comandos para os outros grupos

---

## Conceitos Trabalhados

Durante a aula, foram praticados os seguintes conceitos:

- MQTT com `PubSubClient`
- conexao segura com `WiFiClientSecure`
- autenticacao WPA2 Enterprise com `WPA2_AUTH_PEAP`
- tratamento de reconexao Wi-Fi e MQTT
- controle por topicos individuais e globais
- debounce de botoes
- detecção de borda de descida
- multiplexacao de pinos compartilhados

---

## Dificuldades Encontradas

Ao longo da atividade, alguns problemas comuns apareceram:

- biblioteca `PubSubClient` nao instalada
- falha de conexao MQTT usando host ou porta incorretos
- diferenca entre MQTT TCP/TLS e WebSocket
- LEDs deixando de responder ao compartilhar GPIO com botoes

As solucoes adotadas foram documentadas em [Atividade/README.md](/abs/path/c:/Users/nicolas.337911/Downloads/Internet-of-Things/AULA07/Atividade/README.md:1).

---

## Como Usar Esta Pasta

1. Leia o enunciado e a explicacao pratica em [Atividade/README.md](/abs/path/c:/Users/nicolas.337911/Downloads/Internet-of-Things/AULA07/Atividade/README.md:1).
2. Grave `Atividade.ino` na placa que atuara como receptor.
3. Grave `Emissor.ino` na placa que atuara como transmissor.
4. Ajuste as credenciais do Wi-Fi Enterprise antes do upload.
5. Teste os topicos MQTT com o cliente web do HiveMQ.

---

## Resultado Esperado

Ao final da aula, o sistema deve ser capaz de:

- receber comandos MQTT e controlar LEDs locais
- enviar comandos MQTT a partir de botoes
- operar em rede Enterprise
- demonstrar controle individual, setorial e global

---

## Referencia

Para os detalhes completos da montagem, configuracao e testes, consulte:

- [Atividade/README.md](/abs/path/c:/Users/nicolas.337911/Downloads/Internet-of-Things/AULA07/Atividade/README.md:1)
