#include <Arduino.h>
#include "SensorDHT.h"
#include "Ventilacion.h"
#include "Riego.h"
#include "PantallaOLED.h"

// Pines
#define BUTTON_PIN 14
#define POTENCIOMETRO_PIN 32

// Objetos
SensorDHT sensor(5, DHT22);
Ventilacion ventilacion(4, 25.0);
Riego riego(12);
PantallaOLED oled;

// Variables globales
int pantallaActual = 1;            // 1 = temperatura, 2 = humedad
bool botonAnterior = HIGH;         // estado anterior del botón
unsigned long ultimoCambio = 0;    // tiempo del último cambio
const unsigned long DEBOUNCE = 250; // ms

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    sensor.begin();
    ventilacion.inicializar();
    riego.inicializar();
    riego.generarUmbralAleatorio();
    oled.begin();

    Serial.println("--- Sistema Iniciado ---");
}

void loop() {
    // --- Lecturas ---
    float temp = sensor.leerTemperatura();
    float hum = sensor.leerHumedad();
    float tempRef = map(analogRead(POTENCIOMETRO_PIN), 0, 4095, 0, 40);

    // --- Control ---
    ventilacion.actualizar(temp, tempRef);
    riego.actualizar(hum);

    // --- Lectura del botón con debounce no bloqueante ---
    bool botonActual = digitalRead(BUTTON_PIN);
    if (botonAnterior == HIGH && botonActual == LOW) { // flanco descendente
        unsigned long ahora = millis();
        if (ahora - ultimoCambio > DEBOUNCE) {
            pantallaActual = 3 - pantallaActual; // alterna entre 1 y 2
            ultimoCambio = ahora;
        }
    }
    botonAnterior = botonActual;

    // --- Mostrar en OLED ---
    oled.mostrar(temp, tempRef, ventilacion.getEstado(),
                 hum, riego.getUmbral(), riego.getEstado(), pantallaActual);

    // --- Sin delay largo, loop rápido ---
    delay(50); // muy corto para no bloquear
}