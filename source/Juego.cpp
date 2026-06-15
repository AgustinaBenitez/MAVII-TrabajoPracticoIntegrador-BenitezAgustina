#include "Juego.h"
#include "Pelota.h"
#include "Bumper.h"

#include <raylib.h>

// Implementación del escuchador
EscuchadorColisiones::EscuchadorColisiones() {}

void EscuchadorColisiones::BeginContact(b2Contact* contacto) {

    // Obtengo los cuerpos que chocaron
    b2Body* cuerpoA = contacto->GetFixtureA()->GetBody();
    b2Body* cuerpoB = contacto->GetFixtureB()->GetBody();

    // Recupero los punteros que le pegué al userData en ObjetoFisico.cpp
    ObjetoFisico* objA = reinterpret_cast<ObjetoFisico*>(cuerpoA->GetUserData().pointer);
    ObjetoFisico* objB = reinterpret_cast<ObjetoFisico*>(cuerpoB->GetUserData().pointer);

    // Si existen, les aviso que los golpearon
    if (objA != nullptr) {
        objA->Golpeado();
    }

    if (objB != nullptr) {
        objB->Golpeado();
    }

}

Juego::Juego() {

    // Inicializo gravedad
    b2Vec2 gravedad(0.0f, 150.0f);

    // Inicializo mundo físico
    mundo = std::make_unique<b2World>(gravedad);

}

void Juego::Iniciar() {

    InitWindow(900, 600, "MAVII - Entrega Actividad Integradora - Benitez Agustina");

    InitAudioDevice();

    SetTargetFPS(60);

    // Cargo música de fondo
    //musicaFondo = LoadMusicStream("assets/Loonboon.mp3");
    //musicaFondo.looping = true;     // Para que se repita infinitamente
    //PlayMusicStream(musicaFondo);   // Le doy Play solo acá (una sola vez)

    // Cargo los sonidos
    


    // Configuro el escuchador de colisiones
    escuchador = std::make_unique<EscuchadorColisiones>();
    mundo->SetContactListener(escuchador.get());

    // Cargo todos los objetos
    Reiniciar();

}

void Juego::Actualizar() {

    //UpdateMusicStream(musicaFondo); // Obligatorio para que suene la música

    // Avanzo la simulación física
    mundo->Step(1.0f / 60.0f, 8, 3);

    // Dejo esto por si necesito que algún objeto se mueva
    //if (cosa) {
    //    cosa->Actualizar();
    //}

    // Controles de las paletas
    if (flipperIzq) {
        flipperIzq->Activar(IsKeyDown(KEY_A));
    }

    if (flipperDer) {
        flipperDer->Activar(IsKeyDown(KEY_D));
    }

    // Para reiniciar juego
    if (IsKeyPressed(KEY_R)) {
        Reiniciar();
    }

    // Para mostrar info en pantalla
    if (IsKeyPressed(KEY_SPACE)) {
        modoDebug = !modoDebug;
    }

}

void Juego::Renderizar() {

    BeginDrawing();

        ClearBackground(RAYWHITE);

        // Box2D calcula, Raylib dibuja el resultado visual
        for (const auto& obj : objetos) {
            obj->Dibujar();
        }

        // Dibujo los flippers si existen

        if (flipperIzq) {
            flipperIzq->Dibujar();
        }

        if (flipperDer) {
            flipperDer->Dibujar();
        }

    // Muestro carteles
    DrawText("ESPACIO para explicaciones", 25, 21, 30, DARKGRAY);
    DrawText("R para reiniciar", 25, 61, 30, DARKGRAY);

    // Dejo esto para las explicaciones futuras
    //if (modoDebug) {
    //    if (cosa) trampolin->DibujarDebug();
    //}

    // Para ver las coordenadas
    // Obtengo las posiciones actuales del cursor
    int mouseX = GetMouseX();
    int mouseY = GetMouseY();

    // Dibujo el texto en la esquina superior izquierda de la pantalla
    DrawText(TextFormat("X: %i / Y: %i", mouseX, mouseY), 10, 10, 20, GREEN);

    EndDrawing();

}

void Juego::Reiniciar() {

    // Creo los bordes Estáticos?
    //objetos.emplace_back(std::make_unique<Borde>(mundo.get(), b2Vec2{ 500, 580 }, 0.0f, 1000.0f, 40.0f, b2_staticBody, DARKGRAY));

    // Instancio los objetos

    // Dejo esto para cuando tenga que colocar un objeto
    //cosa = std::make_unique<Cosa>(mundo.get(), b2Vec2{ 500.0f, 500.0f });

    // Limpio el vector por si apretamos la tecla R durante el juego
    objetos.clear();

    
    // Instanciamos los flippers: mundo, posicion, ancho, alto, esIzquierdo
    flipperIzq = std::make_unique<Flipper>(mundo.get(), b2Vec2{ 340.0f, 500.0f }, 100.0f, 35.0f, true);
    flipperDer = std::make_unique<Flipper>(mundo.get(), b2Vec2{ 560.0f, 500.0f }, 100.0f, 35.0f, false);

    // Instancio la pelota y le paso el mundo, posición, radio
    objetos.emplace_back(std::make_unique<Pelota>(mundo.get(), b2Vec2{ 440.0f, 100.0f }, 15.0f, WHITE));

    // Bumper Bulbasaur
    objetos.emplace_back(std::make_unique<Bumper>(mundo.get(), b2Vec2{ 300.0f, 250.0f }, 30.0f,
        "assets/img/texturaBulbasaur.png", "assets/img/texturaIvysaur.png", "assets/img/texturaVenusaur.png",
        "assets/audio/sonidoBulbasaur.wav", "assets/audio/sonidoIvysaur.wav", "assets/audio/sonidoVenasaur.wav"));

    // Bumper Charmander
    objetos.emplace_back(std::make_unique<Bumper>(mundo.get(), b2Vec2{ 600.0f, 250.0f }, 30.0f,
        "assets/img/texturaCharmander.png", "assets/img/texturaCharmeleon.png", "assets/img/texturaCharizard.png",
        "assets/audio/sonidoCharmander.wav", "assets/audio/sonidoCharmeleon.wav", "assets/audio/sonidoCharizard.wav"));

    // Bumper Squirtle
    objetos.emplace_back(std::make_unique<Bumper>(mundo.get(), b2Vec2{ 450.0f, 350.0f }, 30.0f,
        "assets/img/texturaSquirtle.png", "assets/img/texturaWartortle.png", "assets/img/texturaBlastoise.png",
        "assets/audio/sonidoSquirtle.wav", "assets/audio/sonidoWartortle.wav", "assets/audio/sonidoBlastoise.wav"));

}

Juego::~Juego() {

    // Limpio los objetos explícitamente AHORA.
    // Esto fuerza a que todos los DestroyBody se ejecuten mientras el mundo sigue vivo.
    objetos.clear();

    // Descargo los recursos de Raylib
    //UnloadMusicStream(musicaFondo);
    //UnloadSound(sonidoX);

}
