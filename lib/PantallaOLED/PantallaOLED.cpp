#include "PantallaOLED.h"
#include <Wire.h>

PantallaOLED::PantallaOLED() : interfaz(128, 64, &Wire) {}

void PantallaOLED::begin() {
    interfaz.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    interfaz.clearDisplay();
    interfaz.display();
}

const char* PantallaOLED::modoToStr(Modo m) const {
    switch (m) {
        case Modo::AUTO:      return "AUTO";
        case Modo::MANUAL_ON: return "MAN ON";
        case Modo::MANUAL_OFF:return "MAN OFF";
        default:              return "?";
    }
}

void PantallaOLED::mostrar(float temp, float tempRef, bool ventilacion, Modo modoVent,
                           float hum, float humUmbral, bool riego, Modo modoRiego,
                           int pantallaActual,
                           bool refTempAuto, bool refHumAuto) {
    interfaz.clearDisplay();
    interfaz.setTextSize(1);
    interfaz.setTextColor(SSD1306_WHITE);

    if (pantallaActual == 1) {
        // ---- ESTADO ----
        interfaz.setCursor(0,0);  interfaz.println("1) ESTADO");
        interfaz.print("T: "); interfaz.print(temp,1); interfaz.print("C  Ref:");
        interfaz.print(tempRef,1); interfaz.print(refTempAuto? "C(A)" : "C(M)"); interfaz.println();
        interfaz.print("Vent: "); interfaz.print(ventilacion?"ON ":"OFF"); interfaz.print(" ");
        interfaz.println(modoToStr(modoVent));

        interfaz.print("H: "); interfaz.print(hum,1); interfaz.println("%");
        interfaz.print("Umbral: "); interfaz.print(humUmbral); interfaz.print("%");
        interfaz.println(refHumAuto? " (A)" : " (M)");
        interfaz.print("Riego: "); interfaz.print(riego?"ON ":"OFF"); interfaz.print(" ");
        interfaz.println(modoToStr(modoRiego));

    } else if (pantallaActual == 2) {
        // ---- REFERENCIAS ----
        interfaz.setCursor(0,0);  interfaz.println("2) REFERENCIAS");
        interfaz.print("TempRef: "); interfaz.print(tempRef,1);
        interfaz.println(refTempAuto? "C (AUTO)" : "C (MANUAL)");
        interfaz.print("HumUmbral: "); interfaz.print(humUmbral); 
        interfaz.println(refHumAuto? "% (AUTO)" : "% (MANUAL)");

        interfaz.setCursor(0,40);
        interfaz.println("Serie:");
        interfaz.println(" T=25 | T=AUTO");
        interfaz.println(" H=55 | H=AUTO|H=RAND");

    } else {
        // ---- FORZAR ----
        interfaz.setCursor(0,0);  interfaz.println("3) FORZAR");
        interfaz.print("Vent: "); interfaz.println(modoToStr(modoVent));
        interfaz.print("Riego: "); interfaz.println(modoToStr(modoRiego));

        interfaz.setCursor(0,36);
        interfaz.println("Serie:");
        interfaz.println(" V=ON|OFF|AUTO");
        interfaz.println(" R=ON|OFF|AUTO");
    }

    interfaz.display();
}
