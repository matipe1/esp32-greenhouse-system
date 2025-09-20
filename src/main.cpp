#include <Arduino.h>
#include "SensorDHT.h"
#include "Ventilacion.h"
#include "Riego.h"
#include "PantallaOLED.h"

// Pines
#define SENSOR_DHT_PIN 5
#define LED_VENTILACION_PIN 4
#define LED_RIEGO_PIN 12
#define BUTTON_PIN 14
#define POTENCIOMETRO_PIN 32

// Objetos
PantallaOLED oled;
SensorDHT sensor(SENSOR_DHT_PIN, DHT22);
Ventilacion ventilacion(LED_VENTILACION_PIN, 25.0);
Riego riego(LED_RIEGO_PIN);

// Variables globales
int pantallaActual = 1;             // 1 = temperatura, 2 = humedad
bool botonAnterior = HIGH;          // estado anterior del botón
unsigned long ultimoCambio = 0;     // tiempo del último cambio
const unsigned long DEBOUNCE = 250; // ms

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP); // establecemos pin como entrada (pull-up)

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

    // --- Lectura del botón con debounce (sin bloqueo) ---
    bool botonActual = digitalRead(BUTTON_PIN);
    if (botonAnterior == HIGH && botonActual == LOW) {  // flanco descendente
        unsigned long ahora = millis();
        if (ahora - ultimoCambio > DEBOUNCE) {          // ultimo cambio fue hace mas de 250ms
            pantallaActual = 3 - pantallaActual;        // alterna entre 1 y 2
            ultimoCambio = ahora;                       // guardamos tiempo (ms) en que se tocó el botón
        }
    }
    botonAnterior = botonActual;

    // --- Mostrar en OLED ---
    oled.mostrar(temp, tempRef, ventilacion.getEstado(),
                 hum, riego.getUmbral(), riego.getEstado(), pantallaActual);

    // --- Sin delay largo, loop rápido para no bloquear ---
    delay(50);
}