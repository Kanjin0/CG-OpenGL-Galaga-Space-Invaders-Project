#pragma once
#include "ofMain.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras.h"

class Enemy {
public:

    void setup(float x, float y, float z, float sizeX, float sizeY, float sizeZ);
    void update(ofVec3f speedDir);
    void draw();

    ofVec3f pos;
    ofVec3f speed;
    ofVec3f dimensions;
    bool dead;
    int life;
    

};