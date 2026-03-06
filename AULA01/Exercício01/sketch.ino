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

  leitura_dig = !digitalRead(SENSOR_DIG); // Assuming INPUT_PULLUP, so HIGH means not pressed, LOW means pressed
  leitura_an = analogRead(SENSOR_AN);

  estados.ios.sensor.dig = (leitura_dig & 0x01);
  estados.ios.sensor.an = (leitura_an & 0x3FF);
  ldr_percent = (float)100.0 * (float)(leitura_an / (float)SENSOR_MAX);
}

void acende_leds() {
  // Sets the physical LED states according to the values stored in the union
  digitalWrite(LED_VD, estados.ios.led.vd);
  digitalWrite(LED_AM, estados.ios.led.am);
  digitalWrite(LED_VM, estados.ios.led.vm);
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
  pinMode(LED_AM, OUTPUT); // Corrected: Was repeated LED_VD
  pinMode(LED_VM, OUTPUT); // Corrected: Was repeated LED_VD
  pinMode(SENSOR_DIG, INPUT_PULLUP);
  pinMode(SENSOR_AN, INPUT_PULLUP);

  Serial.begin(9600);

  // Inicia desligado
  digitalWrite(LED_VD, 0);
  digitalWrite(LED_AM, 0);
  digitalWrite(LED_VM, 0);

  estados.fullVar = 0; // Initialize all states to zero (LEDs off, sensors not active)
  Serial.println("============================================");
}

void loop() {
  static bool prev_dig_state = false; // To detect button press (rising edge detection)

  leituras_sensor();

  // Check if the digital sensor (button) has just been pressed
  // estados.ios.sensor.dig is true (1) when the button is pressed (LOW due to INPUT_PULLUP and '!')
  if (estados.ios.sensor.dig == true && prev_dig_state == false) {
    // Toggle all LED states
    estados.ios.led.vd = !estados.ios.led.vd;
    estados.ios.led.am = !estados.ios.led.am;
    estados.ios.led.vm = !estados.ios.led.vm;
  }
  prev_dig_state = estados.ios.sensor.dig; // Update previous state for next loop iteration

  acende_leds(); // Update physical LEDs based on the new states in 'estados'

  serial();

  delay(500); // Small delay to debounce and make serial output readable
}