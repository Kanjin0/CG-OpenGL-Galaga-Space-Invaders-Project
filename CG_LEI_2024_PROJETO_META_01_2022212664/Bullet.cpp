#include "Bullet.h"

void Bullet::setup(float x, float y, float z, float sizeX, float sizeY, float sizeZ, ofVec3f speedInit)
{

	pos = ofVec3f(x, y, z);
	dimensions = ofVec3f(sizeX, sizeY, sizeZ);
	speedDir = speedInit;
	accel = ofVec3f(0, 1, 0);
	hasHit = false;
	speedNow = accel * speedDir;
}

void Bullet::update()
{
	if(!hasHit) pos += speedNow;
}

void Bullet::draw()
{
	if (!hasHit) {

		glColor3f(255, 0, 110);
		filled();
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glScalef(dimensions.x, dimensions.y, dimensions.z);
		cube_unit();
		glPopMatrix();

		glColor3f(0, 245, 255);
		wireframe();
		glPushMatrix();
		glTranslatef(pos.x, pos.y, pos.z);
		glScalef(dimensions.x*1.3, dimensions.y*1.3, dimensions.z* 1.3);
		cube_malha_unit(1,1);
		glPopMatrix();

	}
}
