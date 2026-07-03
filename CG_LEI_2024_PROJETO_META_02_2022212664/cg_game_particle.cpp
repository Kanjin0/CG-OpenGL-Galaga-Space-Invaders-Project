#include "cg_game_particle.h"

void Particle::setup(float xmin, float xmax, float ymin, float ymax){
    float x = ofRandom(xmin, xmax);
    float y = ofRandom(ymin, ymax);

    pos = ofVec3f(x, y, 0);
    
    age = 0.;
    lifeTime = int(ofRandom(5, 60)); // Tempo de vida das particulas após a colisão (caso sejam usadas para tal)

    speed = ofVec3f(ofRandom(-4, 4.), ofRandom(-4, 4.), ofRandom(-4, 4.)) *0.1; // vetor que controla a velocidade das particulas em cada
    //direção, ou seja, caso o range do ramdom seja maior, deve gerar uma maior dispersão na colisão em numeros grandes de partículas

    size = gw()*0.01;
    dead = false;
}
void Particle::update(){
    pos += speed;
    age++;
    if(age > lifeTime){
        dead = true;
    }
}
void Particle::draw(){
    if(!dead){
        glColor3f(1., 0., 0.);
        filled();
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(size, size, size);
        cube_unit();
        glPopMatrix();

        glColor3f(0., 0., 0.);
        wireframe();
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(size, size, size);
        cube_unit();
        glPopMatrix();
    }
}