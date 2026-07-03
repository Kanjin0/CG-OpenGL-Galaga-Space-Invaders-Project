#pragma once
#include "ofMain.h"
#include "cg_extras.h"
#include "cg_drawing_extras.h"
#include "cg_cam_extras.h"
#include "cg_game_particle.h"
#include "enemy.h"
#include "Bullet.h"

class Game{
    public:

    Game();
    ~Game();
    void setup();
    void resetPhysics();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void setupGLWorld();

    float avatarSize;
    int state;
    int numStates;

    //day 1 starts here
    int points;

    int gridCols;
    int gridRows;
    float gridMinX;
    float gridMaxX;
    float gridSizeX;
    float gridSizeY;

    float entityScalingX;
    float entityScalingY;
    float entityScalingZ;

    float naveEasing;
    float naveSizeX;
    float naveSizeY;
    float naveSizeZ;
    ofVec3f targetLPos;
    ofVec3f targetRPos;
    ofVec3f target;
    ofVec3f navePos;
    int moveDir;
    bool isMoving;
    void moveNaveHelper(ofVec3f target);
    void moveNave();
    //day 1 ends here

    //day 2 starts here
    bool isStartingScreen;
    bool isShooting;
    void shoot();
    vector<Enemy> wave1;
    vector<Enemy> wave2;
    void makeWave1();
    void makeWave2();
    int frameCounter;
    void moveEnemies();
    int waveCounter;
    vector<Bullet> bulletsMidAir;
    void updateBullets();
    //day 2 ends here

    //day 3 starts here
    void checkCollisions(vector<Enemy> &wave);
    bool winState;
    void checkLoss();
    void checkLossHelper(vector<Enemy>& wave);
    int shootFrameCounter;
    bool canShoot;
    int camState;
    int numCamStates;
    void updateCam();
    //day 3 ends here

    ofVec3f enemyPos;
    vector<Particle> explosion;
    ofVec3f shootPos;
    ofVec3f shootVel;
    float shootSize;

};