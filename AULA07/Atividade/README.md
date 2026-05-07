# Atividade Prática - Malha de Controle Industrial com MQTT

## Sistema

Sistema de controle de uma fábrica com 8 setores.

Cada setor possui 4 LEDs indicadores.

O objetivo é realizar o controle:

- Individual
- Setorial
- Global

---
Hardware:

- #define LED1 4
- #define LED1 0
- #define LED1 2
- #define LED1 15

---

## Configuração do Broker (HiveMQ)

- **Host:** `broker.hivemq.com`
- **Porta (Web):** `8884`
- **Porta (ESP):** `1883`

---

## Padronização de Tópicos

### Exemplo de grupos

- `satc/g1/led1` até `satc/g1/led4`
- `satc/g8/led1` até `satc/g8/led4`

---

## Desafios

### Controle

Cada grupo deve programar seu kit/ESP para:

- Fazer **Subscribe** nos seus 4 tópicos específicos.
- Usar o cliente Web do HiveMQ para enviar `"ON"` e `"OFF"` para o seu `gX/ledY` e verificar a resposta da placa.
- Experimentar controlar os LEDs de outros grupos.

---

## Exemplo de tópicos

| Grupo | Tópico |
|---|---|
| G4 | `satc/g4/led1` |
| G5 | `satc/g5/led1` |
| G6 | `satc/g6/led1` |
| G7 | `satc/g7/led1` |
| G8 | `satc/g8/led1` |