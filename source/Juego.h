#pragma once

#include "ObjetoFisico.h"
#include "Flipper.h"

#include <box2d.h>
#include <vector>
#include <memory>

// Escuchador de colisiones
class EscuchadorColisiones : public b2ContactListener {

public:

    EscuchadorColisiones();
    void BeginContact(b2Contact* contacto) override;

};

class Juego {

private:

    // Mundo físico (contenedor) + objetos
    std::unique_ptr<b2World> mundo;
    std::vector<std::unique_ptr<ObjetoFisico>> objetos;

    // Audio + escuchador
    //Music musicaFondo;
    std::unique_ptr<EscuchadorColisiones> escuchador;

    // Objetos
    std::unique_ptr<Flipper> flipperIzq;
    std::unique_ptr<Flipper> flipperDer;

    // Para mostrar la info solicitada en pantalla sin ensuciar la simulación
    bool modoDebug = false;

public:

    Juego();
    ~Juego();

    void Iniciar();
    void Actualizar();
    void Renderizar();
    void Reiniciar();

};