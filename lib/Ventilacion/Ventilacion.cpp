#include "Ventilacion.h"

Ventilacion::Ventilacion(uint8_t pin, float referencia)
    : pinLed(pin), tempReferencia(referencia), estado(false), modo(Modo::AUTO) {}

void Ventilacion::inicializar() {
    pinMode(pinLed, OUTPUT);
    aplicarSalida(false);
}

void Ventilacion::aplicarSalida(bool on) {
    digitalWrite(pinLed, on ? HIGH : LOW);
    estado = on;
}

void Ventilacion::setReferencia(float ref) { tempReferencia = ref; }

void Ventilacion::setModo(Modo m) {
    modo = m;
    if (modo == Modo::MANUAL_ON)  aplicarSalida(true);
    if (modo == Modo::MANUAL_OFF) aplicarSalida(false);
    // En AUTO no cambiamos nada aquí; lo hará actualizar()
}

void Ventilacion::actualizar(float temperaturaActual) {
    if (modo == Modo::AUTO) {
        aplicarSalida(temperaturaActual > tempReferencia);
    }
    // En manual, el estado ya lo fijó setModo()
}
