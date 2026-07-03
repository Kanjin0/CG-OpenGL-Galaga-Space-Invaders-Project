#include "enemy.h"

void Enemy::setup(float x, float y, float z, float sizeX, float sizeY, float sizeZ)
{
	pos = ofVec3f(x, y, z);

	life = 5;
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
       
        if (life > 3) glColor3f(1, 1, 0);
        else glColor3f(1, 0, 0);
        filled();
        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glScalef(dimensions.x, dimensions.y, dimensions.z);
        cube_unit();
        glPopMatrix();

        
        if (life > 3) glColor3f(1, 0, 0);
        else glColor3f(1, 0, 1);
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
