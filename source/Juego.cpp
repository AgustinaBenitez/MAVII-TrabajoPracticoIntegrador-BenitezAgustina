#include "Juego.h"
#include "Pelota.h"

#include <raylib.h>

Juego::Juego() {

    // Inicializo gravedad
    b2Vec2 gravedad(0.0f, 9.8f);

    // Inicializo mundo físico
    mundo = std::make_unique<b2World>(gravedad);

}

void Juego::Iniciar() {

    InitWindow(900, 600, "MAVII - Entrega Actividad Integradora - Benitez Agustina");

    SetTargetFPS(60);

    // Cargo todos los objetos
    Reiniciar();

}

void Juego::Actualizar() {

    // Avanzo la simulación física
    mundo->Step(1.0f / 60.0f, 8, 3);

    // Dejo esto por si necesito que algún objeto se mueva
    //if (cosa) {
    //    cosa->Actualizar();
    //}



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

    // Dibujo la cosa si existe

    //if (cosa) {
    //    cosa->Dibujar();
    //}

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

    // Instancio la pelota y le paso el mundo, posición, radio
    objetos.emplace_back(std::make_unique<Pelota>(mundo.get(), b2Vec2{ 300.0f, 100.0f }, 15.0f, WHITE));

}

Juego::~Juego() {}
