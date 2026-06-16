#pragma once
#include "ObjetoFisico.h"

class Ascensor : public ObjetoFisico {

private:

    float ancho;
    float alto;
    Texture2D texturaGeodude;
    b2PrismaticJoint* joint;
    float velocidad;

public:

    Ascensor(b2World* mundo, b2Vec2 posicion, float w, float h, bool empiezaSubiendo);
    ~Ascensor();

    void Dibujar() override;

    void Actualizar();

};