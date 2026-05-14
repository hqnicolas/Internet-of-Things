# Sistema IoT de Monitoramento e Controle Inteligente com Node-RED + ESP32

---

## 1. Objetivo Geral

Desenvolver uma solução completa de **Internet das Coisas (IoT)**, integrando dispositivos embarcados, protocolos de comunicação e sistemas de middleware, com foco na aplicação prática de conceitos estudados em sala.

A atividade tem como objetivo principal promover o desenvolvimento de competências relacionadas a:

* Projeto de sistemas distribuídos;
* Integração entre hardware e software;
* Comunicação entre dispositivos em rede;
* Desenvolvimento de aplicações IoT orientadas a cenários reais.

## 2. Descrição da Atividade

Os alunos deverão desenvolver, em duplas, um sistema IoT funcional utilizando a plataforma **ESP32** como dispositivo de borda (*edge*), integrado ao **Node-RED** como ambiente de orquestração e visualização.

A solução deverá contemplar:

* Coleta de dados por sensores;
* Comunicação entre dispositivos;
* Processamento e visualização de dados;
* Implementação de lógica de automação.

## 3. Temas e Aplicações

O sistema desenvolvido deverá simular uma aplicação real. Sugestões de contexto:

* Estufa inteligente;
* Automação residencial;
* Monitoramento ambiental;
* Monitoramento industrial simplificado.
* *Outros temas poderão ser utilizados mediante aprovação do professor.*

## 4. Requisitos Técnicos

### 4.1 Dispositivo ESP32 (Edge)

O sistema deve:

* Coletar dados de, no mínimo, **3 sensores distintos**, tais como:
* Temperatura
* Umidade
* Luminosidade
* Distância


* Implementar comunicação utilizando:
* **MQTT** (obrigatório)
* **CoAP ou HTTP/WebServer** (obrigatório – escolher uma alternativa)


* Permitir o controle de **atuadores** (ex.: LEDs) a partir de comandos externos.

### 4.2 Comunicação

A solução deve demonstrar:

* Uso do modelo *publish/subscribe* (MQTT);
* Uso adicional de um segundo protocolo (CoAP ou HTTP);
* Estrutura organizada de comunicação (tópicos, endpoints, etc.).

### 4.3 Node-RED

Deverá ser implementado:

**a) Interface de visualização (Dashboard)**

* Exibição dos dados dos sensores em tempo real;
* Uso de gráficos, indicadores e/ou medidores.

**b) Lógica de automação**

* Implementação de pelo menos uma regra automatizada, por exemplo:
* *Temperatura acima de determinado valor → acionar LED*
* *Baixa luminosidade → acionar iluminação automática*



**c) Controle manual**

* Interface com controles para envio de comandos ao dispositivo:
* Liga/desliga atuadores;
* Ajuste de parâmetros (opcional).



### 4.4 Integração

O sistema deve garantir comunicação **bidirecional**:

* Envio de dados do **ESP32 → Node-RED**
* Recebimento de comandos do **Node-RED → ESP32**

## 5. Diferenciais (Opcional)

Serão considerados como diferenciais:

* Comparação prática entre protocolos (MQTT vs CoAP/HTTP);
* Persistência de dados (arquivo ou banco de dados);
* Tratamento de falhas (reconexão, robustez);
* Estrutura organizada de código e tópicos;
* Implementação de medidas básicas de segurança.

## 6. Cronograma

| Data | Atividade |
| --- | --- |
| **14/05** | Apresentação da atividade e início do desenvolvimento |
| **21/05** | Desenvolvimento em laboratório |
| **28/05** | Entrega do relatório e realização da prova |

## 7. Entregáveis

### 7.1 Código Fonte

* Código do ESP32 (.ino ou .c);
* Fluxo exportado do Node-RED (.json).

### 7.2 Relatório Técnico

O relatório deve conter:

1. **Introdução:** Contexto do problema e objetivos da solução.
2. **Arquitetura do Sistema:** Diagrama geral, tecnologias utilizadas e justificativas técnicas.
3. **Implementação:** Descrição do funcionamento do ESP32, comunicação entre os componentes e fluxos do Node-RED.
4. **Resultados e Testes:** Evidências de funcionamento, testes realizados e dificuldades encontradas.
5. **Análise de Protocolos:** Comparação entre MQTT e CoAP/HTTP, vantagens e limitações observadas.
6. **Conclusão:** Principais aprendizados e possíveis melhorias.

## 8. Considerações Finais

Espera-se que os alunos desenvolvam não apenas uma solução funcional, mas também demonstrem:

* Capacidade de análise e tomada de decisão técnica;
* Organização e clareza na implementação;
* Visão sistêmica da solução proposta.