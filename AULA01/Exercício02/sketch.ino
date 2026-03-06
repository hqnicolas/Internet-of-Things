#define LED_VD D0
#define LED_AM D3
#define LED_VM D4
#define SENSOR_DIG D5
#define SENSOR_AN A0

#define SENSOR_MAX 1023

union estados {
  uint16_t fullVar;
  struct ios {
    struct sensor {
      int an : 10;  // 0 - 9
      int dig : 1;  // 10
    } sensor;
    struct led {
      int vd : 1;  // 11
      int am : 1;  // 12
      int vm : 1;  // 13
    } led;
    int void14_15 : 2;  // 14 - 15
  } ios;
} estados;

float ldr_percent = 0;

void leituras_sensor() {
  bool leitura_dig = 0;
  int16_t leitura_an = 0;

  leitura_dig = !digitalRead(SENSOR_DIG);
  leitura_an = analogRead(SENSOR_AN);

  estados.ios.sensor.dig = (leitura_dig & 0x01);
  estados.ios.sensor.an = (leitura_an & 0x3FF);
  ldr_percent = (float)100.0 * (float)(leitura_an / (float)SENSOR_MAX);
}

void acende_leds() {
  static bool vd_ant, am_ant, vm_ant;
  bool vd_aux, am_aux, vm_aux;

  vd_aux = estados.ios.led.vd;
  am_aux = estados.ios.led.am;
  vm_aux = estados.ios.led.vm;

  digitalWrite(LED_VD, 1);
  digitalWrite(LED_AM, 1);
  digitalWrite(LED_VM, 1);
}

void serial() {
  Serial.println("LEDS: VD | AM | VM");
  Serial.printf("      %d  | %d  | %d\n\n",
                estados.ios.led.vd & 0x01,
                estados.ios.led.am & 0x01,
                estados.ios.led.vm & 0x01);

  Serial.println("LDR: DIG |  AN  | Percent");
  Serial.printf("      %d  | %4d | %3.2f%%\n",
  estados.ios.sensor.dig & 0x01,
  estados.ios.sensor.an & 0x3FF,
  ldr_percent);

  Serial.println("------------------------");
}

void setup() {
  pinMode(LED_VD, OUTPUT);
  pinMode(LED_VD, OUTPUT);
  pinMode(LED_VD, OUTPUT);
  pinMode(SENSOR_DIG, INPUT_PULLUP);
  pinMode(SENSOR_AN, INPUT_PULLUP);

  Serial.begin(9600);

  // Inicia desligado
  digitalWrite(LED_VD, 0);
  digitalWrite(LED_AM, 0);
  digitalWrite(LED_VM, 0);

  estados.fullVar = 0;
  Serial.println("============================================");
}

void loop() {
  leituras_sensor();

  estados.ios.led.am = 1;
  estados.ios.led.vm = 1;

  acende_leds();

  serial();

  delay(500);
}
