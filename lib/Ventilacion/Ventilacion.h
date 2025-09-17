#ifndef VENTILACION_H
#define VENTILACION_H

#include <Arduino.h>

class Ventilacion {
private:
    uint8_t pinLed;
    float tempReferencia;
    bool estado;

public:
    // Constructor
    Ventilacion(uint8_t pin, float referencia);

    // Inicializa
    void inicializar();

    // Actualiza el estado del ventilador según la temperatura actual y la referencia
    void actualizar(float temperaturaActual, float tempReferencia);

    // Devuelve el estado del ventilador
    bool getEstado() const;
};

#endif