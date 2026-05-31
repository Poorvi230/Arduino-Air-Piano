const int trigPin = 9;
const int echoPin = 10;

// LEDs
const int led1 = 2;
const int led2 = 3;
const int led3 = 4;
const int led4 = 5;
const int led5 = 6;

int currentNote = -1;
int lastNote = -1;

float smoothedDistance = 0;

unsigned long lastDetectionTime = 0;
const int holdTime = 150;

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
}

void loop() {

  // Ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  float distance = duration * 0.034 / 2;

  // Smooth readings
  smoothedDistance =
      (smoothedDistance * 0.9) +
      (distance * 0.1);

  distance = smoothedDistance;

  currentNote = -1;

  // Note mapping
  if (distance >= 10 && distance < 20) {
    currentNote = 60; // C
    showLevel(1);

  } else if (distance >= 20 && distance < 30) {
    currentNote = 62; // D
    showLevel(2);

  } else if (distance >= 30 && distance < 40) {
    currentNote = 64; // E
    showLevel(3);

  } else if (distance >= 40 && distance < 50) {
    currentNote = 65; // F
    showLevel(4);

  } else if (distance >= 50 && distance < 58) {
    currentNote = 67; // G
    showLevel(5);

  } else if (distance >= 58 && distance < 66) {
    currentNote = 69; // A
    pulseLEDs();

  } else if (distance >= 66 && distance < 74) {
    currentNote = 71; // B
    blinkLEDs();

  } else {
    turnOffLEDs();
  }

  // MIDI note logic
  if (currentNote != -1) {

    lastDetectionTime = millis();

    if (currentNote != lastNote) {

      if (lastNote != -1) {
        noteOff(lastNote);
      }

      noteOn(currentNote);
      lastNote = currentNote;
    }
  }

  if (lastNote != -1 &&
    millis() - lastDetectionTime > holdTime) {

  noteOff(lastNote);
  lastNote = -1;

  // Turn all LEDs off
  turnOffLEDs();
}

  delay(120);
}

// LED equalizer
void showLevel(int level) {

  digitalWrite(led1, level >= 1);
  digitalWrite(led2, level >= 2);
  digitalWrite(led3, level >= 3);
  digitalWrite(led4, level >= 4);
  digitalWrite(led5, level >= 5);
}

// A note pulse
void pulseLEDs() {
  for (int i = 0; i < 2; i++) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    delay(60);

    turnOffLEDs();
    delay(60);
  }
}

// B note fast blink
void blinkLEDs() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
    delay(30);

    turnOffLEDs();
    delay(30);
  }
}

void turnOffLEDs() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
}

// MIDI Note ON
void noteOn(int note) {
  Serial.write(0x90);
  Serial.write(note);
  Serial.write(100);
}

// MIDI Note OFF
void noteOff(int note) {
  Serial.write(0x80);
  Serial.write(note);
  Serial.write(0);
}
