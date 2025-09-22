#include "PantallaOLED.h"
#include <Wire.h>

PantallaOLED::PantallaOLED() : interfaz(128, 64, &Wire) {}

void PantallaOLED::begin() {
    interfaz.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    interfaz.clearDisplay();
    interfaz.display();
}

void PantallaOLED::mostrar(float temp, float tempRef, bool ventilacion,
                           float hum, float humUmbral, bool riego, int pantallaActual) {
    interfaz.clearDisplay();
    interfaz.setTextSize(1);
    interfaz.setTextColor(SSD1306_WHITE);

    if (pantallaActual == 1) {
        mostrarEstadoCompleto(temp, tempRef, ventilacion, hum, humUmbral, riego);
    } else if (pantallaActual == 2) {
        interfaz.setCursor(0, 0);
        interfaz.println("Config Manual:");
        interfaz.println("Serial -> T=xx");
        interfaz.println("Serial -> H=xx");
    } else if (pantallaActual == 3) {
        interfaz.setCursor(0, 0);
        interfaz.println("Control Manual:");
        interfaz.println("V=ON/OFF");
        interfaz.println("R=ON/OFF");
    }

    interfaz.display();
}

void PantallaOLED::mostrarEstadoCompleto(float temp, float tempRef, bool ventilacion,
                                         float hum, float humUmbral, bool riego) {
    interfaz.setCursor(0, 0);
    interfaz.print("Temp: "); interfaz.print(temp); interfaz.println("C");
    interfaz.print("Ref: "); interfaz.print(tempRef); interfaz.println("C");
    interfaz.print("Vent: "); interfaz.println(ventilacion ? "ON" : "OFF");

    interfaz.print("Hum: "); interfaz.print(hum); interfaz.println("%");
    interfaz.print("Umbral: "); interfaz.print(humUmbral); interfaz.println("%");
    interfaz.print("Riego: "); interfaz.println(riego ? "ON" : "OFF");
}