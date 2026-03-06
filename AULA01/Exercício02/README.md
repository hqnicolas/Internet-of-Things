# Exercício02 - LED and LDR Control

This project demonstrates basic input/output operations on an Arduino-compatible board, controlling multiple LEDs and reading data from a Light Dependent Resistor (LDR) sensor.

## Features

*   **LED Control**: Manages the state of three LEDs (Green, Yellow, Red).
*   **LDR Sensor Reading**: Reads both digital and analog values from an LDR sensor.
*   **Serial Monitor Output**: Displays the current status of the LEDs and LDR sensor data (digital state, analog value, and estimated percentage) to the serial monitor.
*   **Initialization**: LEDs are set to an OFF state upon startup.

## Hardware Requirements

To run this sketch, you will need:

*   An Arduino-compatible development board (e.g., Arduino Uno, ESP32, etc.)
*   3 LEDs (e.g., Green, Yellow, Red)
*   1 Light Dependent Resistor (LDR)
*   Appropriate resistors for the LEDs and LDR (e.g., pull-up/pull-down for LDR, current limiting for LEDs).
*   Jumper wires
*   Breadboard (optional, for prototyping)

## Software Requirements

*   Arduino IDE
*   Configured board package for your specific Arduino board.

## Setup and Usage

1.  **Circuit Assembly**:
    *   Connect the Green, Yellow, and Red LEDs to digital pins configured as `LED_VD`, `LED_AM`, and `LED_VM` respectively, along with their current-limiting resistors.
    *   Connect the LDR sensor. Its digital output should be connected to the pin configured as `SENSOR_DIG` and its analog output to the pin configured as `SENSOR_AN`. Ensure proper pull-up/pull-down resistor configuration for the LDR.
    *(Note: The specific pin definitions for `LED_VD`, `LED_AM`, `LED_VM`, `SENSOR_DIG`, and `SENSOR_AN` are not provided in `sketch.ino` and would need to be defined in your code.)*

2.  **Upload Sketch**:
    *   Open the `sketch.ino` file in the Arduino IDE.
    *   Select your board and port from the "Tools" menu.
    *   Upload the sketch to your board.

3.  **Serial Monitor**:
    *   Open the Serial Monitor in the Arduino IDE (Tools > Serial Monitor).
    *   Ensure the baud rate is set to `9600`.
    *   You will see output similar to this, indicating the state of LEDs and LDR readings:

    ```
    ============================================
    LEDS: VD | AM | VM
          0  | 0  | 0

    LDR: DIG |  AN  | Percent
          0  |  102 | 10.00%
    ------------------------
    LEDS: VD | AM | VM
          0  | 0  | 0

    LDR: DIG |  AN  | Percent
          1  |  512 | 50.00%
    ------------------------
    ```

## Code Structure

*   **`setup()`**: Initializes serial communication, configures pin modes for LEDs and sensors, and turns off all LEDs initially.
*   **`serial()`**: Formats and prints the current status of LEDs and LDR sensor readings to the serial monitor.
*   **(Missing `loop()` function)**: The provided snippet does not include the `loop()` function, which would typically contain the logic to continuously read sensor data, update LED states (based on the `estados` structure), and call the `serial()` function for periodic output.
*   **`estados` structure**: The code heavily relies on a global `estados` structure (e.g., `estados.ios.led.vd`, `estados.ios.sensor.an`) to manage the state of I/O. The definition of this structure is not provided in the snippet.

## Further Development

*   **Define Pin Assignments**: Add `#define` statements or `const int` variables at the beginning of `sketch.ino` for `LED_VD`, `LED_AM`, `LED_VM`, `SENSOR_DIG`, and `SENSOR_AN`.
*   **Implement `loop()`**: Create a `loop()` function to continuously read sensor data, update the `estados` structure, and control the LEDs based on some logic (e.g., LDR brightness controlling LEDs).
*   **Define `estados` Structure**: Provide the C/C++ definition for the `estados` structure.
*   **Calculate `ldr_percent`**: Implement the logic to calculate `ldr_percent` from the LDR's analog reading.