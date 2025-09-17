#ifndef RIEGO_H
#define RIEGO_H

#include <Arduino.h>

class Riego {
private:
    uint8_t pinLed;
    int humedadUmbral;
    bool estado;
    unsigned long tiempoUltimoCambio;
    const long intervaloParpadeo = 500;

public:
    Riego(uint8_t pin);

    void inicializar();                     // Configura el pin como OUTPUT
    void actualizar(float humedadActual);   // Control intermitente del LED
    void generarUmbralAleatorio();          // Genera umbral aleatorio
    bool getEstado() const;
    int getUmbral() const;
};

#endif