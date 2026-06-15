#pragma once
#include "ObjetoFisico.h"

class Bumper : public ObjetoFisico {

private:

    float radio;

    int contadorGolpes;
    int etapaEvolucion; // Ejemplo: 0=Bulbasaur, 1=Ivysaur, 2=Venusaur

    const char* rutasTexturas[3];
    const char* rutasSonidos[3];

    Texture2D texturaActual;
    Sound sonidoActual;

    // Variables para el efecto visual (se achica ante un golpe)
    int framesEfecto;
    float escalaVisual;

    void Evolucionar();

public:

    Bumper(b2World* mundo, b2Vec2 posicion, float r,
        const char* tex1, const char* tex2, const char* tex3,
        const char* son1, const char* son2, const char* son3);

    ~Bumper();

    void Dibujar() override;
    void Golpeado() override;

};