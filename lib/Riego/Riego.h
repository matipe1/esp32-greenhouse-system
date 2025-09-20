#ifndef RIEGO_H
#define RIEGO_H

#include <Arduino.h>
#include "../common/Modo.h" // o pega enum aquí

class Riego {
private:
    uint8_t pinLed;
    int humedadUmbral;
    bool estado;
    unsigned long tiempoUltimoCambio;
    const long intervaloParpadeo = 500;
    Modo modo;

    void aplicarSalida(bool on);

public:
    Riego(uint8_t pin);

    void inicializar();
    void actualizar(float humedadActual);   // AUTO (parpadeo si bajo umbral)
    void generarUmbralAleatorio();
    void setUmbral(int h);                  // NUEVO
    int  getUmbral() const { return humedadUmbral; }

    void setModo(Modo m);                   // NUEVO
    Modo getModo() const { return modo; }

    bool getEstado() const { return estado; }
};

#endif
