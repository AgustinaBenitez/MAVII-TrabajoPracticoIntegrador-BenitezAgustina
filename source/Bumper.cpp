#include "Bumper.h"

Bumper::Bumper(b2World* mundo, b2Vec2 posicion, float r,
    const char* tex1, const char* tex2, const char* tex3,
    const char* son1, const char* son2, const char* son3)
    : ObjetoFisico(mundo, posicion, 0.0f, b2_staticBody, WHITE) {

    radio = r;

    // Inicializo
    contadorGolpes = 0;
    etapaEvolucion = 0;
    framesEfecto = 0;
    escalaVisual = 1.0f;

    // Guardo las rutas en los arreglos
    rutasTexturas[0] = tex1;
    rutasTexturas[1] = tex2;
    rutasTexturas[2] = tex3;

    rutasSonidos[0] = son1;
    rutasSonidos[1] = son2;
    rutasSonidos[2] = son3;

    // Cargo los assets iniciales
    texturaActual = LoadTexture(rutasTexturas[0]);
    sonidoActual = LoadSound(rutasSonidos[0]);

    // Le doy la forma y seteo propiedades
    b2CircleShape formaBumper;
    formaBumper.m_radius = radio;

    b2FixtureDef fixBumper;
    fixBumper.shape = &formaBumper;
    fixBumper.friction = 0.0f;
    fixBumper.restitution = 50.5f;

    cuerpo->CreateFixture(&fixBumper);

}

Bumper::~Bumper() {

    UnloadTexture(texturaActual);
    UnloadSound(sonidoActual);

}

void Bumper::Golpeado() {

    // Lógica para la evolución
    contadorGolpes++;

    PlaySound(sonidoActual); // Es decir, el que corresponde al bumper golpeado

    // Efecto visual
    framesEfecto = 10; // Lo achico durante 10 frames
    escalaVisual = 0.8f; // Se achica a un 80% de su tamaño

    if (contadorGolpes == 5 && etapaEvolucion == 0) {
        Evolucionar();
    }
    else if (contadorGolpes == 10 && etapaEvolucion == 1) {
        Evolucionar();
    }

}

void Bumper::Evolucionar() {

    if (etapaEvolucion < 2) {

        etapaEvolucion++;

        // Indispensable descargar los recursos viejos para evitar memory leaks ---- Me ayudó Gemini porque yo por mí misma no encontraba por qué los estaba teniendo
        UnloadTexture(texturaActual);
        UnloadSound(sonidoActual);

        // Cargo la nueva textura y sonido leyendo dinámicamente la posición del arreglo
        texturaActual = LoadTexture(rutasTexturas[etapaEvolucion]);
        sonidoActual = LoadSound(rutasSonidos[etapaEvolucion]);

        // El pokémon emitirá su nuevo grito apenas evoluciona
        PlaySound(sonidoActual);

    }

}

void Bumper::Dibujar() {

    b2Vec2 pos = cuerpo->GetPosition();

    // Lógica para el efecto visual de los golpes
    if (framesEfecto > 0) {
        framesEfecto--;
    }
    else {
        escalaVisual = 1.0f;
    }

    Rectangle origen = { 0.0f, 0.0f, (float)texturaActual.width, (float)texturaActual.height };

    // Aplico la escalaVisual al destino para que se achique visualmente
    float diametroVisual = (radio * 2.0f) * escalaVisual;
    Rectangle destino = { pos.x, pos.y, diametroVisual, diametroVisual };

    // El eje de rotación también se ajusta a la escala ---- Sugerido por Gemini
    Vector2 ejeRotacion = { diametroVisual / 2.0f, diametroVisual / 2.0f };

    DrawTexturePro(texturaActual, origen, destino, ejeRotacion, 0.0f, WHITE);

}

bool Bumper::EsEvolucionFinal() {

    return etapaEvolucion == 2; // Devuelve true si es Charizard/Venusaur/Blastoise

}