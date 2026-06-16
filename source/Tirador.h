#pragma once
#include "ObjetoFisico.h"

class Tirador : public ObjetoFisico {

private:

    float ancho;
    float alto;
    Texture2D texturaReposo;
    Texture2D texturaAccion;

    float fuerzaCargada;
    bool yaDisparo;

public:

    Tirador(b2World* mundo, b2Vec2 posicion, float w, float h, const char* rutaReposo, const char* rutaAccion);
    ~Tirador();

    void Dibujar() override;
    void Cargar();
    void Disparar(b2Body* cuerpoPelota);

    bool YaDisparo() { return yaDisparo; }

};