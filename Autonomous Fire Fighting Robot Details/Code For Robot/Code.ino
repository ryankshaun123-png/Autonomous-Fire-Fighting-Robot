#include <Servo.h>

// =============================
// FLAME SENSOR PINS
// =============================
#define FLAME_LEFT    A1
#define FLAME_CENTER  A0
#define FLAME_RIGHT   A2

// =============================
// MOTOR DRIVER PINS
// =============================
#define MOTOR_L1 5
#define MOTOR_L2 6
#define MOTOR_R1 9
#define MOTOR_R2 10

// =============================
// RELAY + SERVO
// =============================
#define RELAY_PIN 8
#define SERVO_PIN 11

Servo waterServo;

bool pumpActive = false;

// =============================
// SETUP
// =============================
void setup() {

    Serial.begin(9600);

    // Motor pins
    pinMode(MOTOR_L1, OUTPUT);
    pinMode(MOTOR_L2, OUTPUT);
    pinMode(MOTOR_R1, OUTPUT);
    pinMode(MOTOR_R2, OUTPUT);

    // Relay setup
    pinMode(RELAY_PIN, OUTPUT);

    // ACTIVE LOW relay
    digitalWrite(RELAY_PIN, HIGH);

    // Servo setup
    waterServo.attach(SERVO_PIN);

    // Center position
    waterServo.write(90);

    // Flame sensors
    pinMode(FLAME_LEFT, INPUT);
    pinMode(FLAME_CENTER, INPUT);
    pinMode(FLAME_RIGHT, INPUT);

    Serial.println("🔥 Fire Fighting Robot Ready");
}

// =============================
// MAIN LOOP
// =============================
void loop() {

    // Read sensors
    int flameLeft   = digitalRead(FLAME_LEFT);
    int flameCenter = digitalRead(FLAME_CENTER);
    int flameRight  = digitalRead(FLAME_RIGHT);

    // Debug values
    Serial.print("L: ");
    Serial.print(flameLeft);

    Serial.print(" | C: ");
    Serial.print(flameCenter);

    Serial.print(" | R: ");
    Serial.println(flameRight);

    // HIGH = Fire detected
    bool fireLeft   = (flameLeft == HIGH);
    bool fireCenter = (flameCenter == HIGH);
    bool fireRight  = (flameRight == HIGH);

    // =============================
    // FIRE DETECTED
    // =============================
    if (fireLeft || fireCenter || fireRight) {

        // -------------------------
        // Fire on LEFT
        // -------------------------
        if (fireLeft && !fireRight) {

            Serial.println("🔥 Fire Left");

            turnLeft();

            stopMotors();

            // Point servo LEFT
            waterServo.write(60);

            activateWaterPump();
        }

        // -------------------------
        // Fire on RIGHT
        // -------------------------
        else if (fireRight && !fireLeft) {

            Serial.println("🔥 Fire Right");

            turnRight();

            stopMotors();

            // Point servo RIGHT
            waterServo.write(120);

            activateWaterPump();
        }

        // -------------------------
        // Fire in CENTER
        // -------------------------
        else if (fireCenter) {

            Serial.println("🔥 Fire Center");

            moveForward();

            delay(300);

            stopMotors();

            // Point servo CENTER
            waterServo.write(90);

            activateWaterPump();
        }

        pumpActive = true;
    }

    // =============================
    // NO FIRE
    // =============================
    else {

        Serial.println("✅ No Fire");

        stopMotors();

        if (pumpActive) {

            deactivateWaterPump();

            pumpActive = false;
        }

        // Keep servo centered
        waterServo.write(90);
    }

    delay(200);
}

// =============================
// MOTOR FUNCTIONS
// =============================

void moveForward() {

    digitalWrite(MOTOR_L1, HIGH);
    digitalWrite(MOTOR_L2, LOW);

    digitalWrite(MOTOR_R1, HIGH);
    digitalWrite(MOTOR_R2, LOW);
}

void turnLeft() {

    digitalWrite(MOTOR_L1, LOW);
    digitalWrite(MOTOR_L2, HIGH);

    digitalWrite(MOTOR_R1, HIGH);
    digitalWrite(MOTOR_R2, LOW);

    delay(250);
}

void turnRight() {

    digitalWrite(MOTOR_L1, HIGH);
    digitalWrite(MOTOR_L2, LOW);

    digitalWrite(MOTOR_R1, LOW);
    digitalWrite(MOTOR_R2, HIGH);

    delay(250);
}

void stopMotors() {

    digitalWrite(MOTOR_L1, LOW);
    digitalWrite(MOTOR_L2, LOW);

    digitalWrite(MOTOR_R1, LOW);
    digitalWrite(MOTOR_R2, LOW);
}

// =============================
// WATER PUMP FUNCTIONS
// =============================

void activateWaterPump() {

    Serial.println("🚰 Pump ON");

    // ACTIVE LOW relay
    digitalWrite(RELAY_PIN, LOW);

    delay(3000);
}

void deactivateWaterPump() {

    Serial.println("🚰 Pump OFF");

    // ACTIVE LOW relay
    digitalWrite(RELAY_PIN, HIGH);
}