#include "Ventilacion.h"
#include <Arduino.h>

Ventilacion::Ventilacion(uint8_t pin, float referenciaInicial)
    : pinLed(pin), estado(false) {}

void Ventilacion::inicializar() {
    pinMode(pinLed, OUTPUT);
}

void Ventilacion::actualizar(float temperaturaActual, float tempReferencia) {
    if (temperaturaActual > tempReferencia) {
        digitalWrite(pinLed, HIGH);
        estado = true;
    } else {
        digitalWrite(pinLed, LOW);
        estado = false;
    }
}

bool Ventilacion::getEstado() const {
    return estado;
}