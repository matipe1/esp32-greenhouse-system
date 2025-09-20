#include "Riego.h"

Riego::Riego(uint8_t pin)
  : pinLed(pin), estado(false), humedadUmbral(50),
    tiempoUltimoCambio(0), modo(Modo::AUTO) {}

void Riego::inicializar() { pinMode(pinLed, OUTPUT); aplicarSalida(false); }

void Riego::aplicarSalida(bool on) { digitalWrite(pinLed, on ? HIGH : LOW); estado = on; }

void Riego::generarUmbralAleatorio() {
    randomSeed(micros() + analogRead(34));
    humedadUmbral = random(40, 61);
    Serial.print("Umbral de humedad generado: ");
    Serial.println(humedadUmbral);
}

void Riego::setUmbral(int h) { humedadUmbral = constrain(h, 0, 100); }

void Riego::setModo(Modo m) {
    modo = m;
    if (modo == Modo::MANUAL_ON)  aplicarSalida(true);
    if (modo == Modo::MANUAL_OFF) aplicarSalida(false);
    // En AUTO lo decidirá actualizar()
}

void Riego::actualizar(float humedadActual) {
    if (modo == Modo::AUTO) {
        unsigned long ahora = millis();
        if (humedadActual < humedadUmbral) {
            // Parpadeo
            if (ahora - tiempoUltimoCambio >= intervaloParpadeo) {
                digitalWrite(pinLed, !digitalRead(pinLed));
                estado = (digitalRead(pinLed) == HIGH);
                tiempoUltimoCambio = ahora;
            }
        } else {
            aplicarSalida(false);
        }
    }
    // En manual, estado ya fue fijado por setModo()
}
