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
int pantallaActual = 1;             // 1 = estado, 2 = config, 3 = control manual
bool botonAnterior = HIGH;          // estado anterior del botón
unsigned long ultimoCambio = 0;     // tiempo del último cambio
const unsigned long DEBOUNCE = 250; // ms

// Para configuraciones manuales
float tempRefManual = -1; // -1 = no usado, usar potenciómetro
int humUmbralManual = -1;
bool controlManual = false;
bool forzarVentilacion = false;
bool forzarRiego = false;

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
    float tempRef = (tempRefManual == -1) ? map(analogRead(POTENCIOMETRO_PIN), 0, 4095, 0, 40) : tempRefManual;
    int humUmbral = (humUmbralManual == -1) ? riego.getUmbral() : humUmbralManual;

    // --- Control automático o manual ---
    if (!controlManual) {
        ventilacion.actualizar(temp, tempRef);
        riego.actualizar(hum);
    } else {
        digitalWrite(LED_VENTILACION_PIN, forzarVentilacion ? HIGH : LOW);
        digitalWrite(LED_RIEGO_PIN, forzarRiego ? HIGH : LOW);
    }

    // --- Botón: cambiar de vista ---
    bool botonActual = digitalRead(BUTTON_PIN);
    if (botonAnterior == HIGH && botonActual == LOW) {      // flanco descendente
        unsigned long ahora = millis();
        if (ahora - ultimoCambio > DEBOUNCE) {              // si el ultimo cambio fue hace 
            pantallaActual++;                               // mas de 250ms alterna entre 1 y 2
            if (pantallaActual > 3) pantallaActual = 1;
            ultimoCambio = ahora;                           // -> guardamos tiempo (ms) en que se tocó el botón
        }
    }
    botonAnterior = botonActual;

    // --- Serial: comandos según vista ---
    if (Serial.available()) {
        String comando = Serial.readStringUntil('\n');
        comando.trim();

        if (pantallaActual == 2) { // Config manual
            if (comando.startsWith("T=")) {
                tempRefManual = comando.substring(2).toFloat();
                Serial.println("Nueva tempRef: " + String(tempRefManual));
            } else if (comando.startsWith("H=")) {
                humUmbralManual = comando.substring(2).toInt();
                Serial.println("Nuevo humUmbral: " + String(humUmbralManual));
            }
        } else if (pantallaActual == 3) { // Control manual
            controlManual = true;
            if (comando == "V=ON") forzarVentilacion = true;
            else if (comando == "V=OFF") forzarVentilacion = false;
            else if (comando == "R=ON") forzarRiego = true;
            else if (comando == "R=OFF") forzarRiego = false;
        }
    }

        // --- Mostrar en OLED ---
    oled.mostrar(temp, tempRef, ventilacion.getEstado(),
                 hum, humUmbral, riego.getEstado(), pantallaActual);

    // --- Sin delay largo, loop rápido para no bloquear ---
    delay(150);
}