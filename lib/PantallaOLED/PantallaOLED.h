#ifndef PANTALLAOLED_H
#define PANTALLAOLED_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class PantallaOLED {
private:
    Adafruit_SSD1306 interfaz;

public:
    PantallaOLED();

    // Inicializa el display
    void begin();

    // Muestra los datos según la pantalla activa
    void mostrar(float temp, float tempRef, bool ventilacion,
                 float hum, float humUmbral, bool riego, int pantallaActual);
};

#endif