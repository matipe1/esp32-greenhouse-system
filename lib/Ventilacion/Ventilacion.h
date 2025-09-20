#ifndef VENTILACION_H
#define VENTILACION_H

#include <Arduino.h>
#include "../common/Modo.h"

class Ventilacion {
private:
    uint8_t pinLed;
    float tempReferencia;
    bool estado;
    Modo modo;

    void aplicarSalida(bool on);

public:
    Ventilacion(uint8_t pin, float referencia);
    void inicializar();

    void setReferencia(float ref);
    float getReferencia() const { return tempReferencia; }

    void setModo(Modo m);
    Modo getModo() const { return modo; }

    void actualizar(float temperaturaActual);

    // 👇 DEFINICIÓN INLINE (nota el "const")
    bool getEstado() const { return estado; }
};

#endif
