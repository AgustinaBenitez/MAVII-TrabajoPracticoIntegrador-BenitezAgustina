#pragma once

#include "ObjetoFisico.h"
#include "Flipper.h"
#include "Tirador.h"
#include "Ascensor.h"

#include <box2d.h>
#include <vector>
#include <memory>

// Escuchador de colisiones
class EscuchadorColisiones : public b2ContactListener {

public:

    EscuchadorColisiones();
    void BeginContact(b2Contact* contacto) override;

};

// Estados del juego
enum EstadoJuego { JUGANDO, GANASTE, PERDISTE };

class Juego {

private:

    EstadoJuego estadoActual;

    // Mundo físico (contenedor) + objetos
    std::unique_ptr<b2World> mundo;
    std::vector<std::unique_ptr<ObjetoFisico>> objetos;

    // Recursos
    Music musicaFondo;
    Texture2D texturaFondo;

    // Listener
    std::unique_ptr<EscuchadorColisiones> escuchador;

    // Objetos
    std::unique_ptr<Flipper> flipperIzq;
    std::unique_ptr<Flipper> flipperDer;
    std::unique_ptr<Tirador> tirador;
    std::unique_ptr<Ascensor> ascensorIzq;
    std::unique_ptr<Ascensor> ascensorDer;
    ObjetoFisico* pelotaPrincipal; // Puntero para rastrear la pelota principal

    // Para mostrar la info solicitada en pantalla sin ensuciar la pantalla de juego
    bool modoDebug = false;

public:

    Juego();
    ~Juego();

    void Iniciar();
    void Actualizar();
    void Renderizar();
    void Reiniciar();

};