#include "enemy.h"
#include "materiais.h"

void Enemy::setup(float x, float y, float z, float sizeX, float sizeY, float sizeZ)
{
	pos = ofVec3f(x, y, z);

	life = 4;
	speed = ofVec3f(1, 1, 1);
	dead = false;
    dimensions = ofVec3f(sizeX, sizeY, sizeZ);
}

void Enemy::update(ofVec3f speedDir)
{

	pos += speed * speedDir;
    if (life == 0) dead = true;
}

void Enemy::draw()
{
	if (!dead) {
       
        if (life > 2) loadMaterial(10);
        else loadMaterial(15);
        filled();
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(dimensions.x, dimensions.y, dimensions.z);
        cube_unit();
        glPopMatrix();

        
        if (life > 2) loadMaterial(15);
        else loadMaterial(3);
        ofSetLineWidth(2);
        wireframe();
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(dimensions.x * 1.3, dimensions.y * 1.3, dimensions.z * 1.3);
        cube_malha_unit(1, 1);
        glPopMatrix();
        ofSetLineWidth(1);
	}
}
