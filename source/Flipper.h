#pragma once
#include "ObjetoFisico.h"

class Flipper : public ObjetoFisico {

private:

    float ancho;
    float alto;
    bool izquierdo;
    Texture2D texturaPuerro;
    b2RevoluteJoint* joint;

public:

    Flipper(b2World* mundo, b2Vec2 posicion, float w, float h, bool esIzquierdo);
    ~Flipper();

    void Dibujar() override;

    // Para mover la paleta desde Juego.cpp
    void Activar(bool presionado);

    void DibujarDebug();

};