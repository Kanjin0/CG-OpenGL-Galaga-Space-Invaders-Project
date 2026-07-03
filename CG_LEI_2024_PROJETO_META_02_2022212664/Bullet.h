#pragma once
#include "ofMain.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras.h"

class Bullet {
public:
    void setup(float x, float y, float z, float sizeX, float sizeY, float sizeZ, ofVec3f speedInit);
    void update();
    void draw();

    ofVec3f pos;
    ofVec3f speedNow;
    ofVec3f speedDir;
    ofVec3f dimensions;
    ofVec3f accel;
    bool hasHit;
};