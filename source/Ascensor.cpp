#include "Ascensor.h"

Ascensor::Ascensor(b2World* mundo, b2Vec2 posicion, float w, float h, bool empiezaSubiendo)
    : ObjetoFisico(mundo, posicion, 0.0f, b2_dynamicBody, WHITE) {

    ancho = w;
    alto = h;

    // Asigno la velocidad inicial según el ascensor
    if (empiezaSubiendo) {
        velocidad = -150.0f;
    }
    else {
        velocidad = 80.0f;
    }

    // Cargo la textura
    texturaGeodude = LoadTexture("assets/img/texturaGeodude.png");

    // Bloqueo la rotación para que el cuerpo se mantenga siempre derecho
    cuerpo->SetFixedRotation(true);

    // Defino el alto de la tabla (aprox el 20% de la imagen)
    float altoTabla = alto * 0.2f;

    // Calculo el centro de esa tablita empujándola hacia el techo de mi rectángulo
    b2Vec2 centroTabla(0.0f, -alto / 2.0f + altoTabla / 2.0f);

    // Creo la forma geométrica pasándole el centro desplazado
    b2PolygonShape formaTabla;
    formaTabla.SetAsBox(ancho / 2.0f, altoTabla / 2.0f, centroTabla, 0.0f);

    // Le doy mucha densidad para que la pelota no lo frene fácil al chocar
    b2FixtureDef fixTabla;
    fixTabla.shape = &formaTabla;
    fixTabla.density = 20.0f;
    fixTabla.friction = 0.5f;
    fixTabla.restitution = 0.2f;
    cuerpo->CreateFixture(&fixTabla);

    // Creo un ancla estática invisible en la misma posición de origen
    b2BodyDef defAncla;
    defAncla.type = b2_staticBody;
    defAncla.position = posicion;
    b2Body* ancla = mundo->CreateBody(&defAncla);

    // Configuro el joint prismático para que se deslice solo en el eje Y (0 en X, 1 en Y)
    b2PrismaticJointDef defJoint;
    b2Vec2 ejeMovimiento(0.0f, 1.0f);
    defJoint.Initialize(ancla, cuerpo, posicion, ejeMovimiento);

    // Le habilito los topes. Le digo que suba hasta 80 píxeles por encima del ancla
    defJoint.enableLimit = true;
    defJoint.lowerTranslation = -80.0f;
    defJoint.upperTranslation = 40.0f;

    // Le enciendo el motor y le doy fuerza bestial para asegurar que venza la gravedad
    defJoint.enableMotor = true;
    defJoint.maxMotorForce = 100000000.0f;
    defJoint.motorSpeed = velocidad;

    joint = (b2PrismaticJoint*)mundo->CreateJoint(&defJoint);

}

Ascensor::~Ascensor() {

    UnloadTexture(texturaGeodude);

}

void Ascensor::Actualizar() {

    // Me fijo cuánto se desplazó el joint desde su punto de origen
    float recorrido = joint->GetJointTranslation();

    // Si chocó contra el límite de arriba, invierto la velocidad para que baje
    if (recorrido <= joint->GetLowerLimit()) {

        velocidad = 80.0f; // Positivo baja, lo hago bajar un poco más lento
        joint->SetMotorSpeed(velocidad);

    }

    // Si volvió al piso, invierto la velocidad para que suba rápido de nuevo
    else if (recorrido >= joint->GetUpperLimit()) {

        velocidad = -150.0f; // Negativo sube rápido
        joint->SetMotorSpeed(velocidad);

    }

}

void Ascensor::Dibujar() {

    b2Vec2 pos = cuerpo->GetPosition();

    Rectangle origen = { 0.0f, 0.0f, (float)texturaGeodude.width, (float)texturaGeodude.height };
    Rectangle destino = { pos.x, pos.y, ancho, alto };
    Vector2 ejeRotacion = { ancho / 2.0f, alto / 2.0f };

    DrawTexturePro(texturaGeodude, origen, destino, ejeRotacion, 0.0f, WHITE);

}