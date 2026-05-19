# Aula 08 - Sistema IoT com Node-RED + ESP32

## Visao Geral

Esta aula propoe o desenvolvimento de um sistema IoT completo com **ESP32** como dispositivo de borda e **Node-RED** como camada de supervisao, visualizacao e automacao.

A entrega deve demonstrar:

- coleta de dados por sensores;
- comunicacao em rede usando **MQTT**;
- uso complementar de **HTTP WebServer**;
- dashboard com dados em tempo real;
- automacao e controle manual;
- comunicacao bidirecional entre **ESP32** e **Node-RED**.

## Objetivos da Aula

- integrar hardware, firmware e middleware em um unico projeto;
- aplicar o modelo `publish/subscribe` com MQTT;
- organizar topicos, endpoints e fluxo de dados;
- validar uma solucao IoT em um cenario proximo de uso real.

## Requisitos Minimos

- usar o **ESP32** como dispositivo de borda;
- coletar pelo menos **3 grandezas monitoradas**;
- implementar **MQTT** obrigatoriamente;
- implementar **HTTP WebServer** como segundo protocolo;
- permitir controle de atuadores a partir de comandos externos;
- construir um dashboard no **Node-RED** com visualizacao e automacao.

Observacao:

- o projeto `Smart Desk Ergonomica IoT` ja contempla `temperatura`, `umidade` e `luminosidade`;
- se a avaliacao exigir **3 modulos fisicos distintos**, adicione um terceiro sensor e replique a mesma estrategia de telemetria.

## Estrutura da Pasta

```text
AULA08/
|-- README.md
`-- Atividade/
    |-- README.md
    `-- ESP32/
        `-- README.md
```

Descricao dos arquivos:

- `AULA08/README.md`: enunciado resumido da aula e fluxo macro de execucao;
- `AULA08/Atividade/README.md`: guia operacional completo do projeto `Smart Desk Ergonomica IoT`;
- `AULA08/Atividade/ESP32/README.md`: referencia especifica do firmware, pinagem e validacao local.

## Como Executar a AULA08

1. Leia o guia da atividade em [Atividade/README.md](./Atividade/README.md) para entender o projeto, os componentes e a estrategia de comunicacao.
2. Monte o hardware com ESP32, sensores, botoes, displays, rele e LEDs de status.
3. Prepare o ambiente com Arduino IDE ou PlatformIO, bibliotecas da placa, Node-RED e acesso ao Wi-Fi `SATC 2.4`.
4. Configure o firmware com as credenciais WPA2 Enterprise, o identificador do grupo e os parametros de MQTT.
5. Implemente e teste localmente a leitura de `temperatura`, `umidade` e `luminosidade`, alem do comportamento dos botoes e LEDs.
6. Ative a comunicacao MQTT para publicar telemetria e receber comandos do Node-RED.
7. Exponha um `HTTP WebServer` no ESP32 para consulta de status e ajuste basico de parametros.
8. Monte o dashboard e a automacao no Node-RED, incluindo visualizacao, controle manual e pelo menos uma regra automatica.
9. Execute os testes finais para comprovar a comunicacao bidirecional, a automacao e o funcionamento completo do sistema.

## Convencoes de Comunicacao

- protocolo principal: **MQTT**
- protocolo complementar: **HTTP WebServer**
- broker sugerido: `broker.hivemq.com`
- porta MQTT segura: `8883`
- rede de laboratorio: `SATC 2.4` com `WPA2 Enterprise`

Os topicos devem seguir um padrao por grupo no estilo da `AULA07`, agora separados por telemetria, comando e status. Os detalhes operacionais e os canais minimos esperados estao documentados em [Atividade/README.md](./Atividade/README.md).

## Cronograma

| Data | Atividade |
| --- | --- |
| **14/05** | Apresentacao da atividade e inicio do desenvolvimento |
| **21/05** | Desenvolvimento em laboratorio |
| **28/05** | Entrega do relatorio e realizacao da prova |

## Entregaveis

### Codigo Fonte

- codigo do ESP32 (`.ino` ou `.c`);
- fluxo exportado do Node-RED (`.json`).

### Relatorio Tecnico

O relatorio deve conter:

1. Introducao com contexto e objetivo da solucao.
2. Arquitetura do sistema com diagrama, tecnologias e justificativas.
3. Implementacao do ESP32, da comunicacao e do fluxo no Node-RED.
4. Resultados, testes e dificuldades encontradas.
5. Analise comparativa entre MQTT e HTTP WebServer.
6. Conclusao com aprendizados e melhorias futuras.

## Leitura Recomendada

- Guia completo da atividade: [Atividade/README.md](./Atividade/README.md)
- Referencia do firmware ESP32: [Atividade/ESP32/README.md](./Atividade/ESP32/README.md)
