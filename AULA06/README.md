# Atividade Prática: Protocolos MQTT e CoAP em IoT

Esta atividade visa aprofundar o conhecimento sobre protocolos de aplicação específicos para Internet das Coisas (IoT), unindo a fundamentação teórica à implementação prática com o microcontrolador **ESP32**.

---

## 1. Contextualização
Na arquitetura de sistemas IoT, os protocolos são comumente organizados em três grandes grupos:

| Camada | Protocolos Exemplos |
| :--- | :--- |
| **Enlace / Rádio** (Física e MAC) | Wi-Fi, Bluetooth/BLE, Zigbee, LoRaWAN, Sigfox, NB-IoT |
| **Rede e Transporte** | TCP/UDP, IPv6, 6LoWPAN |
| **Aplicação** | HTTP/HTTPS, **MQTT**, **CoAP** |

> **Nota:** Até o momento, focamos no uso de Wi-Fi (Enlace), TCP (Transporte) e HTTP (Aplicação) para comunicação síncrona e publicação em plataformas como ThingSpeak.

---

## 2. Objetivos da Atividade
1. Compreender os modelos de comunicação **MQTT** (Publish/Subscribe) e **CoAP** (Request/Response).
2. Analisar as vantagens técnicas de protocolos leves em comparação ao HTTP tradicional.
3. Implementar um protótipo funcional utilizando a placa **ESP32**.

---

## 3. Etapa 1: Pesquisa Teórica (Em Duplas)
Os alunos devem realizar uma pesquisa abordando os seguintes tópicos:

1. **Análise Técnica:** Explicação sobre modelos de comunicação, camadas de transporte utilizadas e cenários de uso.
2. **Quadro Comparativo:** Construir uma tabela comparando **HTTP vs. MQTT vs. CoAP**, contendo pelo menos 5 critérios:
    * Transporte (TCP ou UDP);
    * Confiabilidade (QoS, confirmações);
    * Consumo de energia e overhead;
    * Simplicidade de implementação;
    * Exemplos de uso real.
3. **Conclusão Crítica:** Responder justificadamente: *“Em quais tipos de projeto IoT é mais vantajoso usar MQTT e em quais usar CoAP?”*

---

## 4. Etapa 2: Implementação Prática (ESP32)
Na aula presencial de **07/05/2026**, as duplas deverão escolher uma das trilhas abaixo para desenvolver e demonstrar:

### Trilha 1: MQTT (Publish/Subscribe)
* **Objetivo:** Configurar o ESP32 para publicar dados de sensores (reais ou simulados).
* **Requisitos:**
    * Uso de um Broker (Local: *Mosquitto* ou Público: *broker.emqx.io* / *test.mosquitto.org*).
    * Envio periódico de informações (ex: temperatura/umidade).

### Trilha 2: CoAP (Servidor de Recursos)
* **Objetivo:** Transformar o ESP32 em um servidor CoAP que responda a requisições externas.
* **Requisitos:**
    * Implementação de endpoints (ex: `/led` ou `/sensor`).
    * Resposta em formato leve (JSON ou texto simples) via método GET.

---

## 5. Critérios de Entrega
A entrega final deve ser composta por:

* [ ] **Relatório (PDF):** Contendo a pesquisa teórica e o quadro comparativo.
* [ ] **Código Fonte (.zip):** Código devidamente comentado.
* [ ] **Demonstração:** Apresentação prática de 5 a 10 minutos na data estipulada, relatando as principais dificuldades encontradas.

---

### Dicas de Ferramentas
* **Para MQTT:** MQTT Explorer ou aplicativos mobile para monitorar tópicos.
* **Para CoAP:** Extensão *Copper* (Firefox) ou ferramentas de linha de comando como o `coap-client`.