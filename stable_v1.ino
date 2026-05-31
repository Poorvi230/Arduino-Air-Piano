const int trigPin = 9;
const int echoPin = 10;

// LEDs
const int led1 = 2;
const int led2 = 3;
const int led3 = 4;
const int led4 = 5;
const int led5 = 6;

int currentNote = -1;
int stableNote = -1;
int lastNote = -1;

float smoothedDistance = 0;

unsigned long lastDetectionTime = 0;
const int holdTime = 140;

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

  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000);

  // No hand detected
  if (duration == 0) {
    stopCurrentNote();
    return;
  }

  float distance = duration * 0.034 / 2;

  // Ignore bad readings
  if (distance < 8 || distance > 72) {
    stopCurrentNote();
    return;
  }

  // Balanced smoothing
  smoothedDistance =
      (smoothedDistance * 0.78) +
      (distance * 0.22);

  distance = smoothedDistance;

  currentNote = -1;

  // -------- NOTE ZONES --------

  // C
  if (distance >= 10 && distance < 17) {

    currentNote = 60;
    showLevel(1);

  // D
  } else if (distance >= 17 && distance < 24) {

    if (stableNote == 60 && distance < 19)
      currentNote = 60;
    else
      currentNote = 62;

    showLevel(2);

  // E
  } else if (distance >= 24 && distance < 31) {

    if (stableNote == 62 && distance < 26)
      currentNote = 62;
    else
      currentNote = 64;

    showLevel(3);

  // F
  } else if (distance >= 31 && distance < 38) {

    if (stableNote == 64 && distance < 33)
      currentNote = 64;
    else
      currentNote = 65;

    showLevel(4);

  // G (wider range)
  } else if (distance >= 38 && distance < 48) {

    if (stableNote == 65 && distance < 41)
      currentNote = 65;
    else
      currentNote = 67;

    showLevel(5);

  // A (wider range)
  } else if (distance >= 48 && distance < 58) {

    if (stableNote == 67 && distance < 51)
      currentNote = 67;
    else
      currentNote = 69;

    showLevel(5);

  // B (widest range)
  } else if (distance >= 58 && distance < 70) {

    if (stableNote == 69 && distance < 61)
      currentNote = 69;
    else
      currentNote = 71;

    showLevel(5);

  } else {
    turnOffLEDs();
  }

  // -------- MIDI --------

  if (currentNote != -1) {

    lastDetectionTime = millis();

    if (currentNote != stableNote) {

      stableNote = currentNote;

      if (lastNote != -1) {
        noteOff(lastNote);
      }

      noteOn(currentNote);
      lastNote = currentNote;
    }
  }

  // Hand removed
  if (lastNote != -1 &&
      millis() - lastDetectionTime > holdTime) {

    stopCurrentNote();
  }

  delay(20);
}

// -------- LED FUNCTIONS --------

void showLevel(int level) {

  digitalWrite(led1, level >= 1);
  digitalWrite(led2, level >= 2);
  digitalWrite(led3, level >= 3);
  digitalWrite(led4, level >= 4);
  digitalWrite(led5, level >= 5);
}

void turnOffLEDs() {

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
}

// -------- MIDI --------

void noteOn(int note) {

  Serial.write(0x90);
  Serial.write(note);
  Serial.write(110);
}

void noteOff(int note) {

  Serial.write(0x80);
  Serial.write(note);
  Serial.write(0);
}

void stopCurrentNote() {

  if (lastNote != -1) {
    noteOff(lastNote);
  }

  lastNote = -1;
  stableNote = -1;

  turnOffLEDs();
}
