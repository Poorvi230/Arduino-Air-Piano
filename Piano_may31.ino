const int trigPin = 9;
const int echoPin = 10;

int currentNote = -1;
int lastNote = -1;

float smoothedDistance = 0;

unsigned long lastDetectionTime = 0;
const int holdTime = 200; // milliseconds

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Read sensor
  long duration = pulseIn(echoPin, HIGH);

  float distance = duration * 0.034 / 2;

  // Smooth sensor readings
  smoothedDistance =
      (smoothedDistance * 0.8) +
      (distance * 0.2);

  distance = smoothedDistance;

  currentNote = -1;

  // Note mapping
  if (distance >= 10 && distance < 20) {
    currentNote = 60; // C

  } else if (distance >= 20 && distance < 30) {
    currentNote = 62; // D

  } else if (distance >= 30 && distance < 40) {
    currentNote = 64; // E

  } else if (distance >= 40 && distance < 50) {
    currentNote = 65; // F

  } else if (distance >= 50 && distance < 58) {
    currentNote = 67; // G

  } else if (distance >= 58 && distance < 66) {
    currentNote = 69; // A

  } else if (distance >= 66 && distance < 74) {
    currentNote = 71; // B
  }

  // If note detected
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

  // Only turn off after short delay
  if (lastNote != -1 &&
      millis() - lastDetectionTime > holdTime) {

    noteOff(lastNote);
    lastNote = -1;
  }

  delay(80);
}

// MIDI Note ON
void noteOn(int note) {
  Serial.write(0x90);
  Serial.write(note);
  Serial.write(120);
}

// MIDI Note OFF
void noteOff(int note) {
  Serial.write(0x80);
  Serial.write(note);
  Serial.write(0);
}