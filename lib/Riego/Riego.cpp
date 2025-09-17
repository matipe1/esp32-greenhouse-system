#include "Riego.h"
#include <Arduino.h>

Riego::Riego(uint8_t pin) : pinLed(pin), estado(false), humedadUmbral(50), tiempoUltimoCambio(0) {}

void Riego::inicializar() {
    pinMode(pinLed, OUTPUT);
}

void Riego::generarUmbralAleatorio() {
    randomSeed(micros() + analogRead(34));
    humedadUmbral = random(40, 61);
    Serial.print("Umbral de humedad generado: ");
    Serial.println(humedadUmbral);
}

void Riego::actualizar(float humedadActual) {
    unsigned long ahora = millis();
    if (humedadActual < humedadUmbral) {
        estado = true;
        if (ahora - tiempoUltimoCambio >= intervaloParpadeo) {
            digitalWrite(pinLed, !digitalRead(pinLed));
            tiempoUltimoCambio = ahora;
        }
    } else {
        estado = false;
        digitalWrite(pinLed, LOW);
    }
}

bool Riego::getEstado() const {
    return estado;
}

int Riego::getUmbral() const {
    return humedadUmbral;
}