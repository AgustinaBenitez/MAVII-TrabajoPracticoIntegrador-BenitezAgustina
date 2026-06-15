#include "Flipper.h"

Flipper::Flipper(b2World* mundo, b2Vec2 posicion, float w, float h, bool esIzquierdo)
    : ObjetoFisico(mundo, posicion, 0.0f, b2_dynamicBody, WHITE) {

    ancho = w;
    alto = h;
    izquierdo = esIzquierdo;

    texturaPuerro = LoadTexture("assets/img/texturaPuerro.png");

    // Armo la forma rectangular de la paleta que es más chica que los bordes de la imagen
    b2Vec2 vertices[4];

    if (izquierdo) {

        // Defino los 4 puntos en sentido anti-horario alrededor del centro (0,0)
        vertices[0].Set(-ancho / 2.0f, alto / 6.0f);   // Abajo izquierda
        vertices[1].Set(ancho / 2.5f, alto / 6.0f);    // Abajo derecha
        vertices[2].Set(ancho / 2.5f, -alto / 6.0f);   // Arriba derecha
        vertices[3].Set(-ancho / 2.0f, -alto / 6.0f);  // Arriba izquierda
    
    }
    else {

        // Lo mismo pero invertido para el flipper derecho
        vertices[0].Set(ancho / 2.0f, alto / 6.0f);
        vertices[1].Set(-ancho / 2.5f, alto / 6.0f);
        vertices[2].Set(-ancho / 2.5f, -alto / 6.0f);
        vertices[3].Set(ancho / 2.0f, -alto / 6.0f);
    
    }

    // Le paso el array de vértices y le digo cuántos son (4)
    b2PolygonShape formaFlipper;    
    formaFlipper.Set(vertices, 4);

    // Propiedades
    b2FixtureDef fixFlipper;
    fixFlipper.shape = &formaFlipper;
    fixFlipper.density = 0.1f;
    fixFlipper.friction = 0.2f;
    cuerpo->CreateFixture(&fixFlipper);

    // Creo un cuerpo estático invisible para que sirva de ancla en la mesa
    b2BodyDef defAncla;
    defAncla.type = b2_staticBody;
    defAncla.position = posicion;
    b2Body* ancla = mundo->CreateBody(&defAncla);

    // Uno el ancla estática con la paleta dinámica usando el Revolute Joint
    b2RevoluteJointDef defJoint;
    defJoint.bodyA = ancla;
    defJoint.bodyB = cuerpo;

    // Si es izquierdo, el ancla va a la izquierda del rectángulo. Si no, a la derecha.
    float offsetAncla;

    if (izquierdo) {
        offsetAncla = -(ancho / 2.0f);
    }
    else {
        offsetAncla = (ancho / 2.0f);
    }

    defJoint.localAnchorA.Set(0.0f, 0.0f);
    defJoint.localAnchorB.Set(offsetAncla, 0.0f);

    // Configuro los topes para que no me gire 360 grados
    defJoint.enableLimit = true;

    if (izquierdo) {

        defJoint.lowerAngle = -0.5f; // Ángulo de reposo (abajo)
        defJoint.upperAngle = 0.5f;  // Ángulo máximo de golpe (arriba)

    }
    else {

        defJoint.lowerAngle = -0.5f; // Invertido para la derecha
        defJoint.upperAngle = 0.5f;

    }

    // Motor del joint (apagado por defecto)
    defJoint.enableMotor = true;
    defJoint.maxMotorTorque = 100000000.0f; // Fuerza bruta del paletazo
    defJoint.motorSpeed = 0.0f;

    joint = (b2RevoluteJoint*)mundo->CreateJoint(&defJoint);

}

Flipper::~Flipper() {

    UnloadTexture(texturaPuerro);

}

void Flipper::Activar(bool presionado) {

    float velocidad;
    
    if (presionado) {
        if (izquierdo) {
            velocidad = -30.0f; // Fuerza hacia arriba (anti-horario)
        }
        else {
            velocidad = 30.0f;  // Fuerza hacia arriba (horario)
        }
    }
    else {
        if (izquierdo) {
            velocidad = 15.0f;  // Vuelve a caer
        }
        else {
            velocidad = -15.0f; // Vuelve a caer
        }
    }

    joint->SetMotorSpeed(velocidad);

}

void Flipper::Dibujar() {

    b2Vec2 pos = cuerpo->GetPosition();
    float angulo = cuerpo->GetAngle() * RAD2DEG;

    // Si no es el izquierdo, le paso el ancho en negativo para espejar la imagen
    float anchoTextura;
    
    if (izquierdo) {
        anchoTextura = (float)texturaPuerro.width;
    }
    else {
        anchoTextura = -(float)texturaPuerro.width; // Negativo para espejar
    }

    Rectangle origen = { 0.0f, 0.0f, anchoTextura, (float)texturaPuerro.height };
    Rectangle destino = { pos.x, pos.y, ancho, alto };

    Vector2 ejeRotacion = { ancho / 2.0f, alto / 2.0f };

    DrawTexturePro(texturaPuerro, origen, destino, ejeRotacion, angulo, WHITE);

}