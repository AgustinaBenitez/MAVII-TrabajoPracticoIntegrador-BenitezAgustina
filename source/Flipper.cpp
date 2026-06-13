#include "Flipper.h"

Flipper::Flipper(b2World* mundo, b2Vec2 posicion, float w, float h, bool esIzquierdo)
    : ObjetoFisico(mundo, posicion, 0.0f, b2_dynamicBody, WHITE) {

    ancho = w;
    alto = h;
    izquierdo = esIzquierdo;

    texturaPuerro = LoadTexture("assets/img/texturaPuerro.png");

    // Armo la forma rectangular de la paleta
    b2PolygonShape formaFlipper;
    formaFlipper.SetAsBox(ancho / 2.0f, alto / 2.0f);

    b2FixtureDef fixFlipper;
    fixFlipper.shape = &formaFlipper;
    fixFlipper.density = 10.0f; // Bastante pesado para que pegue con fuerza
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

    // Configuramos el motor del joint (apagado por defecto)
    defJoint.enableMotor = true;
    defJoint.maxMotorTorque = 5000.0f; // Fuerza bruta del paletazo
    defJoint.motorSpeed = 0.0f;

    joint = (b2RevoluteJoint*)mundo->CreateJoint(&defJoint);

}

Flipper::~Flipper() {

    UnloadTexture(texturaPuerro);

}

void Flipper::Activar(bool presionado) {

    // Si toco la tecla, el motor gira hacia arriba.
    // Si suelto, el motor gira en reversa para volver a la posición de reposo.
    float velocidad;
    
    if (presionado) {
        velocidad = 20.0f;
    }
    else {
        velocidad = -10.0f;
    }
    
    // Invierto la dirección si es el flipper derecho
    if (!izquierdo) velocidad = -velocidad;

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
        anchoTextura = -(float)texturaPuerro.width; // Espejamos la imagen
    }

    Rectangle origen = { 0.0f, 0.0f, anchoTextura, (float)texturaPuerro.height };
    Rectangle destino = { pos.x, pos.y, ancho, alto };

    // El eje de rotación visual tiene que coincidir con el offset de Box2D
    float ejeX;
    
    if (izquierdo) {
        ejeX = 0.0f;
    }
    else {
        ejeX = ancho;
    }

    Vector2 ejeRotacion = { ejeX, alto / 2.0f };

    DrawTexturePro(texturaPuerro, origen, destino, ejeRotacion, angulo, WHITE);

}