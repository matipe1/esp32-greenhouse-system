#include <Arduino.h>
#include "SensorDHT.h"
#include "Ventilacion.h"
#include "Riego.h"
#include "PantallaOLED.h"
#include "../lib/common/Modo.h" // o elimina si pegaste enum en headers

// Pines
#define SENSOR_DHT_PIN 5
#define LED_VENTILACION_PIN 4
#define LED_RIEGO_PIN 12
#define BUTTON_PIN 14
#define POTENCIOMETRO_PIN 32

// Objetos
PantallaOLED oled;
SensorDHT sensor(SENSOR_DHT_PIN, DHT22);
Ventilacion ventilacion(LED_VENTILACION_PIN, 25.0f);
Riego riego(LED_RIEGO_PIN);

// --- Menu ---
enum class MenuItem : uint8_t { ESTADO = 1, REFERENCIAS = 2, FORZAR = 3 };
int pantallaActual = (int)MenuItem::ESTADO; // 1..3

// Botón (debounce)
bool botonAnterior = HIGH;
unsigned long ultimoCambio = 0;
const unsigned long DEBOUNCE = 250; // ms

// Referencias: AUTO (pot/algoritmo) o MANUAL (fijadas por Serial)
bool refTempAuto = true;
float tempRefManual = 25.0f;

bool refHumAuto  = true; // si AUTO, usamos el umbral interno actual del riego
// si piden RAND en AUTO, re-genera; si MANUAL, usamos setUmbral()

// --- Serial parsing ---
String inputLine;

void printHelp() {
    Serial.println(F("=== MENU SERIAL ==="));
    Serial.println(F("  (Menu 2) Referencias:"));
    Serial.println(F("    T=25     -> TempRef manual 25 C (desactiva pot)"));
    Serial.println(F("    T=AUTO   -> TempRef vuelve a pot"));
    Serial.println(F("    H=55     -> Umbral humedad manual 55 %"));
    Serial.println(F("    H=AUTO   -> Umbral vuelve a AUTO (mantiene valor actual)"));
    Serial.println(F("    H=RAND   -> Umbral aleatorio (40..60)"));
    Serial.println(F("  (Menu 3) Forzar actuadores:"));
    Serial.println(F("    V=ON|OFF|AUTO"));
    Serial.println(F("    R=ON|OFF|AUTO"));
    Serial.println(F("  Generales:"));
    Serial.println(F("    M or HELP -> mostrar esta ayuda"));
}

void onEnterLine(const String& lineRaw) {
    String line = lineRaw;
    line.trim();
    line.toUpperCase();
    if (line.length() == 0) return;

    if (line == "M" || line == "HELP" || line == "?") {
        printHelp();
        return;
    }

    // REFERENCIAS
    if (line.startsWith("T=")) {
        String v = line.substring(2);
        v.trim();
        if (v == "AUTO") {
            refTempAuto = true;
            Serial.println(F("TempRef -> AUTO (pot)"));
        } else {
            float tf = v.toFloat();
            if (tf > -100 && tf < 150) {
                tempRefManual = tf;
                refTempAuto = false;
                Serial.print(F("TempRef manual -> "));
                Serial.print(tempRefManual, 1);
                Serial.println(F(" C"));
            } else {
                Serial.println(F("Valor de T invalido"));
            }
        }
        return;
    }

    if (line.startsWith("H=")) {
        String v = line.substring(2);
        v.trim();
        if (v == "AUTO") {
            refHumAuto = true;
            Serial.println(F("Umbral humedad -> AUTO"));
        } else if (v == "RAND") {
            riego.generarUmbralAleatorio();
            refHumAuto = true; // sigue en AUTO con nuevo valor
        } else {
            int h = v.toInt();
            if (h >= 0 && h <= 100) {
                riego.setUmbral(h);
                refHumAuto = false;
                Serial.print(F("Umbral humedad manual -> "));
                Serial.print(h);
                Serial.println(F(" %"));
            } else {
                Serial.println(F("Valor de H invalido (0..100)"));
            }
        }
        return;
    }

    // FORZAR
    if (line.startsWith("V=")) {
        String v = line.substring(2); v.trim();
        if (v == "ON")  ventilacion.setModo(Modo::MANUAL_ON);
        else if (v == "OFF") ventilacion.setModo(Modo::MANUAL_OFF);
        else ventilacion.setModo(Modo::AUTO);
        Serial.print(F("Ventilacion -> "));
        Serial.println(v);
        return;
    }

    if (line.startsWith("R=")) {
        String v = line.substring(2); v.trim();
        if (v == "ON")  riego.setModo(Modo::MANUAL_ON);
        else if (v == "OFF") riego.setModo(Modo::MANUAL_OFF);
        else riego.setModo(Modo::AUTO);
        Serial.print(F("Riego -> "));
        Serial.println(v);
        return;
    }

    Serial.println(F("Comando no reconocido. Escribe M para ver ayuda."));
}

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    sensor.begin();
    ventilacion.inicializar();
    riego.inicializar();
    riego.generarUmbralAleatorio();
    oled.begin();

    Serial.println(F("--- Sistema Iniciado ---"));
    printHelp();
}

void loop() {
    // --- Lecturas ---
    float temp = sensor.leerTemperatura();
    float hum  = sensor.leerHumedad();

    // Referencia de temperatura: pot o manual
    float tempRef = 0.0f;
    if (refTempAuto) {
        tempRef = map(analogRead(POTENCIOMETRO_PIN), 0, 4095, 0, 40);
        ventilacion.setReferencia(tempRef);
    } else {
        tempRef = tempRefManual;
        ventilacion.setReferencia(tempRefManual);
    }

    // --- Control actuadores ---
    ventilacion.actualizar(temp);
    riego.actualizar(hum);

    // --- Boton (debounce) para navegar menu 1..3 ---
    bool botonActual = digitalRead(BUTTON_PIN);
    if (botonAnterior == HIGH && botonActual == LOW) {
        unsigned long ahora = millis();
        if (ahora - ultimoCambio > DEBOUNCE) {
            pantallaActual++;
            if (pantallaActual > 3) pantallaActual = 1;
            ultimoCambio = ahora;
        }
    }
    botonAnterior = botonActual;

    // --- OLED ---
    oled.mostrar(temp, tempRef, ventilacion.getEstado(), ventilacion.getModo(),
                 hum, riego.getUmbral(), riego.getEstado(), riego.getModo(),
                 pantallaActual, refTempAuto, refHumAuto);

    // --- Serial no bloqueante ---
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (inputLine.length() > 0) {
                onEnterLine(inputLine);
                inputLine = "";
            }
        } else {
            inputLine += c;
            if (inputLine.length() > 80) inputLine.remove(0); // evita overflow
        }
    }

    delay(30);
}
