#ifndef PANTALLAOLED_H
#define PANTALLAOLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "../common/Modo.h" // o pega enum aquí

class PantallaOLED {
private:
    Adafruit_SSD1306 interfaz;
    const char* modoToStr(Modo m) const;

public:
    PantallaOLED();
    void begin();

    // pantallaActual: 1 Estado, 2 Referencias, 3 Forzar
    void mostrar(float temp, float tempRef, bool ventilacion, Modo modoVent,
                 float hum, float humUmbral, bool riego, Modo modoRiego,
                 int pantallaActual,
                 bool refTempAuto, bool refHumAuto);
};

#endif
