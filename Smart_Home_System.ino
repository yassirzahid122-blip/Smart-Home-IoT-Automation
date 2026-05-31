#include <DHT.h>

// ---------- DHT11 ----------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------- Security LEDs ----------
#define DOOR_LED 7
#define WINDOW_LED 8

// ---------- Climate LEDs / Fan ----------
#define FURNACE_LED 5
#define AC_LED 6
#define FAN_PIN 9

// ---------- Door / Window Sensors ----------
#define DOOR_SENSOR 2
#define WINDOW_SENSOR 3

// ---------- Variables ----------
String received = "";

bool doorLocked = false;
bool windowLocked = false;

float desiredTemp = 25.0;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(DOOR_LED, OUTPUT);
  pinMode(WINDOW_LED, OUTPUT);
  pinMode(FURNACE_LED, OUTPUT);
  pinMode(AC_LED, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  digitalWrite(DOOR_LED, LOW);
  digitalWrite(WINDOW_LED, LOW);
  digitalWrite(FURNACE_LED, LOW);
  digitalWrite(AC_LED, LOW);
  analogWrite(FAN_PIN, 0);

  pinMode(DOOR_SENSOR, INPUT_PULLUP);
  pinMode(WINDOW_SENSOR, INPUT_PULLUP);
}

void loop() {
  int doorState = digitalRead(DOOR_SENSOR);
  int windowState = digitalRead(WINDOW_SENSOR);

  // Door/window status
  Serial.println(doorState == LOW ? "DOOR OPEN" : "DOOR CLOSED");
  Serial.println(windowState == LOW ? "WINDOW OPEN" : "WINDOW CLOSED");

  // Temperature/humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("TEMP:");
    Serial.println(temperature);

    Serial.print("HUM:");
    Serial.println(humidity);

    // Climate control
    float diff = abs(temperature - desiredTemp);

    if (temperature < desiredTemp) {
      digitalWrite(FURNACE_LED, HIGH);
    } else {
      digitalWrite(FURNACE_LED, LOW);
    }

    if (temperature > desiredTemp + 5) {
      digitalWrite(AC_LED, HIGH);
    } else {
      digitalWrite(AC_LED, LOW);
    }

    if (diff > 5) {
      analogWrite(FAN_PIN, 255);
      Serial.println("FAN:100%");
    }
    else if (diff > 1) {
      analogWrite(FAN_PIN, 128);
      Serial.println("FAN:50%");
    }
    else {
      analogWrite(FAN_PIN, 0);
      Serial.println("FAN:OFF");
    }
  }

  // Intruder alert
  if (doorLocked && doorState == LOW) {
    Serial.println("INTRUDER ALERT: DOOR");

    digitalWrite(DOOR_LED, HIGH);
    digitalWrite(WINDOW_LED, HIGH);
    delay(200);
    digitalWrite(DOOR_LED, LOW);
    digitalWrite(WINDOW_LED, LOW);
    delay(200);
  }
  else if (windowLocked && windowState == LOW) {
    Serial.println("INTRUDER ALERT: WINDOW");

    digitalWrite(DOOR_LED, HIGH);
    digitalWrite(WINDOW_LED, HIGH);
    delay(200);
    digitalWrite(DOOR_LED, LOW);
    digitalWrite(WINDOW_LED, LOW);
    delay(200);
  }

  // Bluetooth commands
  if (Serial.available() > 0) {
    received = Serial.readStringUntil('\n');
    received.trim();

    // Desired temperature command
    if (received.startsWith("T:")) {
      String tempText = received.substring(2);
      tempText.trim();

      desiredTemp = tempText.toFloat();

      Serial.print("Desired Temp Set:");
      Serial.println(desiredTemp);
    }

    else if (received == "LD") {
      if (doorState == LOW) {
        Serial.println("FAIL: DOOR OPEN");
      } else {
        digitalWrite(DOOR_LED, HIGH);
        doorLocked = true;
        Serial.println("Door Locked");
      }
    }

    else if (received == "UD") {
      digitalWrite(DOOR_LED, LOW);
      doorLocked = false;
      Serial.println("Door Unlocked");
    }

    else if (received == "LW") {
      if (windowState == LOW) {
        Serial.println("FAIL: WINDOW OPEN");
      } else {
        digitalWrite(WINDOW_LED, HIGH);
        windowLocked = true;
        Serial.println("Window Locked");
      }
    }

    else if (received == "UW") {
      digitalWrite(WINDOW_LED, LOW);
      windowLocked = false;
      Serial.println("Window Unlocked");
    }

    else if (received == "LB") {
      if (doorState == LOW) {
        Serial.println("FAIL: DOOR OPEN");
      }
      else if (windowState == LOW) {
        Serial.println("FAIL: WINDOW OPEN");
      }
      else {
        digitalWrite(DOOR_LED, HIGH);
        digitalWrite(WINDOW_LED, HIGH);
        doorLocked = true;
        windowLocked = true;
        Serial.println("Both Locked");
      }
    }

    else if (received == "UB") {
      digitalWrite(DOOR_LED, LOW);
      digitalWrite(WINDOW_LED, LOW);
      doorLocked = false;
      windowLocked = false;
      Serial.println("Both Unlocked");
    }

    else {
      Serial.print("Unknown Command:");
      Serial.println(received);
    }
  }

  delay(1000);
}
