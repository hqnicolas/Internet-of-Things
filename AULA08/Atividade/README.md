
# Projeto: Smart Desk Ergonômica IoT

**Foco:** Produtividade, Saúde Ocupacional e Eficiência Energética.

Este projeto transforma uma mesa de trabalho comum em um sistema inteligente que monitora o ambiente, gerencia ciclos de concentração (técnica Pomodoro) e automatiza a iluminação para reduzir a fadiga ocular.

---

## 1. Arquitetura do Hardware (Dispositivo de Borda)

O **ESP32** atuará como o cérebro da estação, processando os sensores e controlando a interface local.

### Entradas (Sensores e Botões)

* **DHT11:** Monitoramento de temperatura e umidade relativa do ar para garantir o conforto térmico.
* **LDR (Analógica):** Medição da luminosidade sobre a superfície de trabalho.
* **4 Botões Digitais:**
1. **Start/Pause:** Inicia ou pausa o cronômetro Pomodoro.
2. **Reset:** Reinicia o ciclo de trabalho.
3. **Toggle Display:** Alterna o que é exibido nos displays (Tempo -> Temp -> Umidade).
4. **Manual Overide:** Liga/desliga o relé (luminária) manualmente.


### Saídas (Atuadores e Interface)

* **2 Displays de 7 Segmentos (Multiplexados):** Exibição de valores de 00 a 99 (minutos do timer ou valores dos sensores).
* **1 Relé:** Controla uma luminária de mesa de 110V/220V ou fita LED de alta potência.
* **4 LEDs de Status:**
* **LED 1 (Verde):** Modo Foco ativo.
* **LED 2 (Amarelo):** Hora da Pausa (Break).
* **LED 3 (Azul):** Conexão MQTT/Wi-Fi ativa.
* **LED 4 (Vermelho):** Alerta ambiental (Ex: Temperatura muito alta ou ar muito seco).

---

## 2. Estratégia de Comunicação

Para cumprir as exigências da disciplina, utilizaremos dois protocolos distintos:

1. **MQTT (Obrigatório):**
* **Publicação:** O ESP32 envia a cada 30 segundos os dados de Temperatura, Umidade e Luminosidade para tópicos como `sensor/quarto/temp`.
* **Subscrição:** O ESP32 escuta o tópico `comando/desk/luz` para que o Node-RED possa acionar o relé remotamente.


2. **HTTP WebServer (Obrigatório - Alternativa):**
* O ESP32 hospedará uma página HTML simples (acessível pelo IP do dispositivo) que exibe o status em tempo real e permite configurar o tempo do Pomodoro (ex: mudar de 25 para 50 minutos) sem precisar do Node-RED.



---

## 3. Lógica de Automação e Node-RED

O **Node-RED** funcionará como o orquestrador inteligente do sistema.

### **Dashboard (Interface de Visualização)**

* **Gauges (Medidores):** Visualização analógica da temperatura e umidade.
* **Gráfico de Histórico:** Acompanhamento da luminosidade ao longo do dia.
* **Controle de Produtividade:** Um contador de quantos ciclos Pomodoro foram concluídos no dia.

### **Regras de Automação (Exemplos)**

1. **Iluminação Inteligente:** Se `Luminosidade < 400` (valor do LDR) **E** `Modo Foco = Ativo`, então `Relé = ON`.
2. **Alerta de Saúde:** Se a `Umidade < 30%`, o Node-RED envia um comando para o **LED Vermelho** piscar no ESP32, avisando o usuário para beber água ou ligar um umidificador.
3. **Registro de Dados:** Persistência dos tempos de foco em um arquivo `.csv` ou log no Node-RED para análise de performance semanal.

---

## 4. Diferenciais Técnicos

* **Multiplexação dos Displays:** Uso de técnica de varredura rápida para controlar os dois dígitos de 7 segmentos usando menos pinos (aprox. 7 pinos de segmentos + 2 pinos de controle de dígito).
* **Robustez de Conexão:** Implementação de lógica de reconexão automática caso o Broker MQTT caia, garantindo que o timer local continue funcionando offline.
* **Sincronização de Tempo:** O ESP32 busca a hora certa via protocolo NTP para garantir que o cronômetro seja preciso.

---