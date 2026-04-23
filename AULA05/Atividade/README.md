#  IoT Smart Control Center - ESP32

Este projeto consiste no desenvolvimento de um sistema de monitoramento e controle remoto utilizando um ESP32. O sistema integra a leitura de sensores ambientais, controle de atuadores via interface web responsiva e feedback visual através de displays de 7 segmentos.

##  Objetivo do Projeto
Implementar um servidor web assíncrono para monitorar sensores (Temperatura, Umidade e Luz) e controlar LEDs (Digitais e RGB) em tempo real, garantindo a interoperabilidade entre hardware e interface de usuário.

---

##  Requisitos Técnicos

### 1. Infraestrutura de Rede & Web
- [ ] **Webserver Assíncrono:** Implementação utilizando a biblioteca `ESPAsyncWebServer`.
- [ ] **Acessibilidade:** Servidor acessível via IP do ESP32 no navegador.
- [ ] **Interface Responsiva:** Design *mobile-first* (compatível com smartphones e desktops).
- [ ] **Logs do Sistema:** Monitoramento via Serial Monitor (Status do WiFi, IP e requisições HTTP).

### 2. Monitoramento de Sensores (Tempo Real)
Atualização automática de dados a cada **2 segundos**:
- [ ] **DHT11:** Leitura de Temperatura (°C) e Umidade (%).
- [ ] **LDR:** Leitura de Luminosidade via pino analógico.
- [ ] **Visualização:** Exibição dos dados em cards intuitivos na interface web.

### 3. Feedback Visual (Hardware)
- [ ] **Displays 7 Segmentos:** Exibição da temperatura atual.
    - 1 dígito para dezenas.
    - 1 dígito para unidades.
    - Exemplo: `25` para 25°C.

### 4. Controle de Atuadores via Web
####  LEDs Digitais (ON/OFF)
Controle individual com feedback visual de estado na interface, usando os pinos reais da shield:
- **LED Vermelho:** GPIO 4
- **LED Verde:** GPIO 0
- **LED Azul:** GPIO 2

####  LED RGB (Controle Analógico)
Controle de intensidade via Sliders (0-255) com pré-visualização de cor:
- **Canal R:** GPIO 25
- **Canal G:** GPIO 26
- **Canal B:** GPIO 27

###  Mapeamento do Display de 7 Segmentos
Hardware utilizado nesta atividade:
- **SEG_A:** GPIO 18
- **SEG_B:** GPIO 5
- **SEG_C:** GPIO 21
- **SEG_D:** GPIO 3
- **SEG_E:** GPIO 1
- **SEG_F:** GPIO 23
- **SEG_G:** GPIO 22
- **SEG_DP:** GPIO 19
- **Display Dezena:** GPIO 16
- **Display Unidade:** GPIO 17

---

##  Design da Interface (UI/UX)
A interface deve ser intuitiva e moderna, composta por:
- **Cards de Sensores:** Organizados para leitura rápida.
- **Botões de Ação:** Grandes, coloridos e táteis.
- **Sliders RGB:** Deslizantes suaves para transição de cores.

---

##  Plano de Implementação (Sugestão)

Para garantir a estabilidade do projeto, recomenda-se a seguinte ordem de desenvolvimento:

1. **Fase 1: Conectividade** $\rightarrow$ Configuração do WiFi e servidor web básico.
2. **Fase 2: Leitura de Dados** $\rightarrow$ Integração do DHT11 e LDR $\rightarrow$ Exibição no Browser.
3. **Fase 3: Atuadores Simples** $\rightarrow$ Controle dos LEDs digitais (GPIOs).
4. **Fase 4: Controle Avançado** $\rightarrow$ Implementação dos Sliders PWM para o LED RGB.
5. **Fase 5: Feedback Local** $\rightarrow$ Lógica de multiplexação/controle dos displays 7 segmentos.
6. **Fase 6: Refinamento** $\rightarrow$ Ajustes de CSS para responsividade e polimento da UI.

---

##  Bibliotecas Necessárias
- `ESPAsyncWebServer`
- `AsyncTCP`
- `DHT sensor library` (Adafruit)
