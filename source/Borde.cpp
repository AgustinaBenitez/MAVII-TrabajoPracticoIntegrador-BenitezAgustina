#include "Borde.h"

Borde::Borde(b2World* mundo, b2Vec2 posicion, float w, float h, float angulo)
    : ObjetoFisico(mundo, posicion, angulo, b2_staticBody, RED) {

    ancho = w;
    alto = h;

    b2PolygonShape formaBorde;
    formaBorde.SetAsBox(ancho / 2.0f, alto / 2.0f);

    b2FixtureDef fixBorde;
    fixBorde.shape = &formaBorde;
    fixBorde.friction = 0.1f;    // Poca fricción para que la pelota no se frene al rozar
    fixBorde.restitution = 0.3f; // Un rebote leve contra la pared

    cuerpo->CreateFixture(&fixBorde);

}

Borde::~Borde() {}

void Borde::Dibujar() {

    ////////// PARA DEBUG MÍO
     
    // Extraigo la forma geométrica de Box2D para dibujar solo los contornos en rojo
    //b2Fixture* fixture = cuerpo->GetFixtureList();

    // Hago una lógica similar a la de las cajitas para las paletas
    //if (fixture != nullptr) {

        //b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
        //int conteoVertices = shape->m_count;
        //b2Transform transformacion = cuerpo->GetTransform();

        //for (int i = 0; i < conteoVertices; i++) {

            //int indiceSiguiente;

            //if (i + 1 < conteoVertices) {
                //indiceSiguiente = i + 1;
            //}
            //else {
                //indiceSiguiente = 0;
            //}

            // Convierto las coordenadas
            //b2Vec2 verticeActualBox2D = b2Mul(transformacion, shape->m_vertices[i]);
            //b2Vec2 verticeSiguienteBox2D = b2Mul(transformacion, shape->m_vertices[indiceSiguiente]);

            //Vector2 verticeActualRaylib = { verticeActualBox2D.x, verticeActualBox2D.y };
            //Vector2 verticeSiguienteRaylib = { verticeSiguienteBox2D.x, verticeSiguienteBox2D.y };

            // Trazo una línea roja para mi debug
            //DrawLineEx(verticeActualRaylib, verticeSiguienteRaylib, 2.0f, RED);
        
        //}
    
    //}

}