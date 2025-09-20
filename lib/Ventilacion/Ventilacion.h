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
    // Constructor
    Ventilacion(uint8_t pin, float referencia);

    // Inicializa
    void inicializar();

    void setReferencia(float ref);
    float getReferencia() const { return tempReferencia; }

    void setModo(Modo m);
    Modo getModo() const { return modo; }

    // Actualiza el estado del ventilador según la temperatura actual y la referencia
    void actualizar(float temperaturaActual);

    // Devuelve el estado del ventilador
    bool getEstado() const;
};

#endif