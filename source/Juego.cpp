#include "Juego.h"
#include "Borde.h"
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

    // Cargo imagen de fondo
    texturaFondo = LoadTexture("assets/img/texturaFondo.png");

    // Cargo música de fondo
    musicaFondo = LoadMusicStream("assets/audio/musicaFondo.mp3");
    musicaFondo.looping = true;     // Para que se repita infinitamente
    PlayMusicStream(musicaFondo);   // Le doy Play solo acá (una sola vez)

    // Configuro el escuchador de colisiones
    escuchador = std::make_unique<EscuchadorColisiones>();
    mundo->SetContactListener(escuchador.get());

    // Cargo todos los objetos
    Reiniciar();

}

void Juego::Actualizar() {

    UpdateMusicStream(musicaFondo); // Obligatorio para que suene la música

    if (estadoActual == JUGANDO) {

        // Sugerencia de Gemini: En lugar de dar 1 paso grande de 1/60, le hacemos dar 10 mini-pasos de 1/600
        // Esto engaña a Box2D y multiplica por 10 su límite de velocidad tope
        for (int i = 0; i < 10; i++) {
            mundo->Step(1.0f / 600.0f, 8, 3);
        }

        // Controles de las paletas
        if (flipperIzq) flipperIzq->Activar(IsKeyDown(KEY_A));
        if (flipperDer) flipperDer->Activar(IsKeyDown(KEY_L));
        if (ascensorIzq) ascensorIzq->Actualizar();
        if (ascensorDer) ascensorDer->Actualizar();

        if (tirador) {
            if (IsKeyDown(KEY_SPACE)) tirador->Cargar();
            if (IsKeyReleased(KEY_SPACE)) tirador->Disparar(pelotaPrincipal->GetCuerpo());
        }

        // CHEQUEO DE VICTORIA Y DERROTA
        b2Vec2 posPelota = pelotaPrincipal->GetCuerpo()->GetPosition();

        //Derrota
        
        // Si se va la pelota por debajo de la pantalla (Y > 620)
        if (posPelota.y > 620.0f) {
            estadoActual = PERDISTE;
        }

        // Si vuelve con Spoink (atrapada en el pasillo derecho)
        // Solo chequeo esto si Spoink YA disparó, porque sino pierdo apenas abro el juego
        if (tirador->YaDisparo() && posPelota.x > 645.0f && posPelota.y > 450.0f) {
            estadoActual = PERDISTE;
        }

        // Victoria (cuento cuántos están en evolución 2)
        int bumpersAlMaximo = 0;

        for (const auto& obj : objetos) {

            if (obj->EsEvolucionFinal()) {
                bumpersAlMaximo++;
            }

        }

        // Si los 3 pokemones llegaron al máximo, gano
        if (bumpersAlMaximo >= 3) {

            estadoActual = GANASTE;

        }

    }

    // Para reiniciar juego
    if (IsKeyPressed(KEY_R)) {
        Reiniciar();
    }

    // Para mostrar info en pantalla
    if (IsKeyPressed(KEY_I)) {
        modoDebug = !modoDebug;
    }

}

void Juego::Renderizar() {

    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(texturaFondo, -3, -37, WHITE);

        // Box2D calcula, Raylib dibuja
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

        // Dibujo los ascensores
        if (ascensorIzq) {
            ascensorIzq->Dibujar();
        }

        if (ascensorDer) {
            ascensorDer->Dibujar();
        }

        // Dibujo el tirador si existe
        if (tirador) {
            tirador->Dibujar();
        }

        // Carteles de victoria y derrota
        if (estadoActual == PERDISTE) {

            DrawText("Perdiste! Presiona R para volver a empezar", 130, 280, 30, RED);
        
        }
        else if (estadoActual == GANASTE) {

            DrawText("Siii, lograste evolucionar todos tus", 120, 260, 35, DARKGREEN);
            DrawText("pokemones! Felicitaciones!", 180, 310, 35, DARKGREEN);
            DrawText("Presiona R para volver a jugar", 230, 380, 25, GRAY);
        
        }

        // Muestro carteles
        //DrawText("ESPACIO para explicaciones", 25, 21, 30, DARKGRAY);
        //DrawText("R para reiniciar", 25, 61, 30, DARKGRAY);

    // Dejo esto para las explicaciones futuras
    //if (modoDebug) {
    //    if (cosa) cosa->DibujarDebug();
    //}

    // Para ver las coordenadas
    // Obtengo las posiciones actuales del cursor
    //int mouseX = GetMouseX();
    //int mouseY = GetMouseY();

    // Dibujo el texto en la esquina superior izquierda de la pantalla
    //DrawText(TextFormat("X: %i / Y: %i", mouseX, mouseY), 10, 10, 20, GREEN);

    EndDrawing();

}

void Juego::Reiniciar() {

    // Para que cada vez que se presione la tecla R, el estado se resetee
    estadoActual = JUGANDO;

    // Limpio el vector por si toco la tecla R durante el juego
    objetos.clear();
    
    // Creo los bordes estáticos
    // Techo (Posicionado en Y = -50 para que quede oculto arriba, con 100 de grosor)
    objetos.emplace_back(std::make_unique<Borde>(mundo.get(), b2Vec2{ 450.0f, -50.0f }, 1000.0f, 100.0f, 0.0f));

    // Pared Izquierda
    objetos.emplace_back(std::make_unique<Borde>(mundo.get(), b2Vec2{ 150.0f, 300.0f }, 100.0f, 800.0f, 0.0f));

    // Pared Derecha
    objetos.emplace_back(std::make_unique<Borde>(mundo.get(), b2Vec2{ 750.0f, 300.0f }, 100.0f, 800.0f, 0.0f));

    // Esquina superior derecha (Plano inclinado para desviar el tiro inicial hacia la izquierda)
    // El ángulo está en radianes: 0.785f equivale a 45 grados.
    objetos.emplace_back(std::make_unique<Borde>(mundo.get(), b2Vec2{ 750.0f, 50.0f }, 200.0f, 100.0f, 0.785f));

    // Esquina superior izquierda
    objetos.emplace_back(std::make_unique<Borde>(mundo.get(), b2Vec2{ 150.0f, 50.0f }, 200.0f, 100.0f, -0.785f));

    // Pared divisoria del pasillo de lanzamiento (Entre el Geodude y Spoink)
    objetos.emplace_back(std::make_unique<Borde>(mundo.get(), b2Vec2{ 650.0f, 430.0f }, 5.0f, 310.0f, 0.0f));

    ////// Instancio los objetos

    // Tirador (Spoink) en pasillo derecho
    tirador = std::make_unique<Tirador>(mundo.get(), b2Vec2{ 676.0f, 550.0f }, 40.0f, 60.0f, "assets/img/texturaSpoink01.png", "assets/img/texturaSpoink02.png");

    // Pelota justo arriba de Spoink
    auto nuevaPelota = std::make_unique<Pelota>(mundo.get(), b2Vec2{ 676.0f, 480.0f }, 15.0f, WHITE);

    // Guardo el puntero para que el tirador sepa a quién dispararle---- Sugerido por Gemini
    pelotaPrincipal = nuevaPelota.get();

    // Meto la pelota en vector
    objetos.emplace_back(std::move(nuevaPelota));

    // Flippers
    flipperIzq = std::make_unique<Flipper>(mundo.get(), b2Vec2{ 325.0f, 520.0f }, 85.0f, 30.0f, true);
    flipperDer = std::make_unique<Flipper>(mundo.get(), b2Vec2{ 525.0f, 520.0f }, 85.0f, 30.0f, false);

    // Ascensores (Geodudes) en los pasillos laterales
    ascensorIzq = std::make_unique<Ascensor>(mundo.get(), b2Vec2{ 257.0f, 530.0f }, 110.0f, 42.0f, true);
    ascensorDer = std::make_unique<Ascensor>(mundo.get(), b2Vec2{ 590.0f, 530.0f }, 110.0f, 42.0f, false);

    // Bumper Bulbasaur
    objetos.emplace_back(std::make_unique<Bumper>(mundo.get(), b2Vec2{ 300.0f, 150.0f }, 30.0f,
        "assets/img/texturaBulbasaur.png", "assets/img/texturaIvysaur.png", "assets/img/texturaVenusaur.png",
        "assets/audio/sonidoBulbasaur.wav", "assets/audio/sonidoIvysaur.wav", "assets/audio/sonidoVenasaur.wav"));

    // Bumper Charmander
    objetos.emplace_back(std::make_unique<Bumper>(mundo.get(), b2Vec2{ 600.0f, 150.0f }, 30.0f,
        "assets/img/texturaCharmander.png", "assets/img/texturaCharmeleon.png", "assets/img/texturaCharizard.png",
        "assets/audio/sonidoCharmander.wav", "assets/audio/sonidoCharmeleon.wav", "assets/audio/sonidoCharizard.wav"));

    // Bumper Squirtle
    objetos.emplace_back(std::make_unique<Bumper>(mundo.get(), b2Vec2{ 450.0f, 250.0f }, 30.0f,
        "assets/img/texturaSquirtle.png", "assets/img/texturaWartortle.png", "assets/img/texturaBlastoise.png",
        "assets/audio/sonidoSquirtle.wav", "assets/audio/sonidoWartortle.wav", "assets/audio/sonidoBlastoise.wav"));

}

Juego::~Juego() {

    // Limpio los objetos explícitamente AHORA ---- Sugerencia de Gemini
    // Esto fuerza a que todos los DestroyBody se ejecuten mientras el mundo sigue vivo.
    objetos.clear();

    // Descargo los recursos de Raylib
    UnloadMusicStream(musicaFondo);
    UnloadTexture(texturaFondo);
    //UnloadSound(sonidoX);

}
