# Arduino Air Piano 🎹
A gesture-controlled air piano built using an Arduino UNO and an HC-SR04 ultrasonic sensor. Hand movement in the air is converted into musical notes and played through MIDI.

## Features

* Gesture-controlled note detection
* Ultrasonic distance sensing
* MIDI sound output
* Adjustable note mapping
* LMMS instrument support

## Hardware Used

* Arduino UNO R3-compatible board
* HC-SR04 Ultrasonic Sensor
* Breadboard
* LED
* 330Ω resistor *(Orange–Orange–Brown–Gold)*
* Jumper wires
* USB cable

## Software Used

* Arduino IDE
* Hairless MIDI Serial Bridge
* loopMIDI
* LMMS

## Wiring

### HC-SR04

* VCC → 5V
* GND → GND
* TRIG → Pin 9
* ECHO → Pin 10

### LED

* Long leg (+) → Pin 3
* Short leg (-) → 330Ω resistor → GND

## Note Mapping

* 10–20 cm → C
* 20–30 cm → D
* 30–40 cm → E
* 40–50 cm → F
* 50–58 cm → G
* 58–66 cm → A
* 66–74 cm → B

## How It Works

The ultrasonic sensor detects hand distance and maps different ranges to musical notes. Arduino sends MIDI signals to the laptop, where LMMS generates instrument sounds.

**Flow:**
Arduino → Hairless MIDI → loopMIDI → LMMS → Speakers

## Project Setup

![Setup](images/setup.jpeg)

![Updated Setup](images/setup2.0.jpeg)

## Future Improvements

* Piano / Synth / Theremin modes
* Push-button mode switching
* Better note stability
* Multiple instrument support

Author

Poorvi
