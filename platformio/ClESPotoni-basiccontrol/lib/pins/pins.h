#pragma once
#include <Arduino.h>

const uint8_t PIN_BUZZ=16;
const uint8_t PIN_MOTOR_SLEEP=2;
const uint8_t PIN_MOTOR_FORWARD_LEFT=4;
const uint8_t PIN_MOTOR_BACKWARD_LEFT=5;
const uint8_t PIN_MOTOR_BACKWARD_RIGHT=12;
const uint8_t PIN_MOTOR_FORWARD_RIGHT=13;
const uint8_t PIN_HEAD_LED=14;

void init_pins() {
    // HeadLED
    pinMode(PIN_HEAD_LED, OUTPUT);
    digitalWrite(PIN_HEAD_LED, LOW);

    // Buzzer
    pinMode(PIN_BUZZ, OUTPUT);
    digitalWrite(PIN_BUZZ, LOW);

    // Motor
    pinMode(PIN_MOTOR_FORWARD_LEFT, OUTPUT);
    pinMode(PIN_MOTOR_BACKWARD_LEFT, OUTPUT);
    pinMode(PIN_MOTOR_BACKWARD_RIGHT, OUTPUT);
    pinMode(PIN_MOTOR_FORWARD_RIGHT, OUTPUT);
    pinMode(PIN_MOTOR_SLEEP, OUTPUT);
    digitalWrite(PIN_MOTOR_FORWARD_LEFT, LOW);
    digitalWrite(PIN_MOTOR_BACKWARD_LEFT, LOW);
    digitalWrite(PIN_MOTOR_BACKWARD_RIGHT, LOW);
    digitalWrite(PIN_MOTOR_FORWARD_RIGHT, LOW);
    digitalWrite(PIN_MOTOR_SLEEP, HIGH);
}