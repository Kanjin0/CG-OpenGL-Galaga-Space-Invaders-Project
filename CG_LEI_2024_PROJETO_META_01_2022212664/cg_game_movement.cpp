#include "cg_game_movement.h"


Game::Game()
{
    setup();
}

Game::~Game()
{
}

void Game::setup()
{
    state = 0;
    numStates = 2;
    numCamStates = 3;
    avatarSize = gw() * 0.1;

    // definição dos tamanhos de scaling para encaixe na grelha
    entityScalingX = gw() * 0.5;
    entityScalingY = gh() * 0.7;
    entityScalingZ = entityScalingX;

    // formato da grelha e tamanhos de cada casa
    gridCols = 9;
    gridRows = 7;
    gridSizeX = entityScalingX / gridRows;
    gridSizeY = entityScalingY / gridCols;
    gridMinX = gridSizeX * -3;
    gridMaxX = gridSizeX * 3;
    
    // tamanhos e posição da nave
    naveSizeX = entityScalingX * .05;
    naveSizeY = entityScalingY * .05;
    naveSizeZ = entityScalingZ * .05;
    navePos = { 0.f,-4 * gridSizeY,0.f };
    
    moveDir = 0;

    // target positions para fazer easing do movimento da nave
    targetLPos = ofVec3f(navePos[0] - gridSizeX,  navePos[1],0 );
    targetRPos = ofVec3f(navePos[0] + gridSizeX,  navePos[1],0 );
    naveEasing = 0.25;
    
    shootVel = ofVec3f(0., 0.2 * gridSizeY, 0.);
    shootSize = gw() * 0.005;
    
    // enemy waves
    
    resetPhysics();
}

void Game::resetPhysics()
{   
    points = 0;
    makeWave1();
    makeWave2();
    frameCounter = 0;
    shootFrameCounter = 0;
    waveCounter = 1;

    isMoving = false;
    isShooting = false;
    isStartingScreen = true;
    winState = true;
    canShoot = false;

    enemyPos = ofVec3f(0, 0, 0);

    navePos = ofVec3f(0., -4.*gridSizeY , 0.);
    targetLPos = ofVec3f(navePos.x - gridSizeX , navePos.y, navePos.z);
    targetRPos = ofVec3f(navePos.x + gridSizeX, navePos.y, navePos.z);

    bulletsMidAir.clear();
    shootPos = navePos;
    camState = 0;
}

void Game::setupGLWorld()
{
    // definição do mundo, neste caso foram colocados os eixos x e y exatamente no centro da tela, como uma cruz que a divide em 4 partes
    // o sentido do eixo X é da esquerda para a direita e o sentido do eixo Y é de baixo para cima
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, -10000, 10000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // lookat(0, 0, 1, 0, 0, 0, 0, 1, 0);
}


// neste apenas estão as funçoes de cada tipo de movimento
void Game::update()
{
    if (state == 1 && winState)
    {
        shootFrameCounter++;
        if (shootFrameCounter >= 15) canShoot = true;
        frameCounter++;
        moveNave();
        shoot();
        if (frameCounter % 180 == 0) {
            moveEnemies();
            checkLoss();
        }
        if (frameCounter % 5 == 0) {
            if (waveCounter == 1) checkCollisions(wave1);
            else checkCollisions(wave2);
            updateBullets();
        }
        if (wave1.empty()) waveCounter++;
        if (wave1.empty() && wave2.empty()) state++;
    }
    
}  


// aqui encontra-se o settup e desenho das figuras que serão afetadas pelo movimento assim como a renderização de outros objetos a desenhar, como as particulas
void Game::draw()
{

    // aparentemente, as coordenadas dadas para os draw String abaixo continuam a condizer com o normal para o OpenFrameworks, mesmo depois do
    // glOrtho do setup
    switch (state)
    {
    case 0:
        ofSetColor(255, 0, 255);
        ofDrawBitmapString("WELCOME TO THE GAME\nPRESS UP TO START", (gw() - 19 * 8) / 2, (gh() - 16) / 2);
        break;
    case 1:
        ofSetColor(255, 0, 0);
        ofDrawBitmapString("PONTUACAO = " + ofToString(points), 30, 30);
        break;
    case 2:
        ofSetColor(255, 0, 255);
        if(winState) ofDrawBitmapString("WELL DONE!\nYOU WON!", (gw() - 11 * 8) / 2, (gh() - 16) / 2);
        else ofDrawBitmapString("OOPS! THE INVADERS GOT YOU!\nDON'T WORRY, YOU STILL MADE " + ofToString(points) + " POINTS!\nPRESS UP TO TRY AGAN!", (gw() - 39 * 8) / 2, (gh() - 24) / 2);
        break;
    }
    // vai ser dividido em 3 estados: 0 - tela inicial (talvez guardar highscores e mostrar um ranking num dos lados); 1 - playing, onde o jogo vai acontecer; 
    // 2 - game over, onde é mostrada a pontuação final (talvez juntamente com um ranking) e se quer jogar de novo
    
    if (state == 1)
    {
        
        updateCam();

        glColor3f(1., 1., 1);
        wireframe();
        glPushMatrix();
        glTranslatef(0, 0, 0);
        glScalef(entityScalingX, entityScalingY, entityScalingZ);
        malha_unit(gridRows, gridCols);
        glPopMatrix();

        glColor3f(0., 1., 0.);
        filled();
        glPushMatrix();
        glTranslatef(navePos.x, navePos.y, navePos.z);
        glScalef(naveSizeX, naveSizeY, naveSizeZ);
        cube_unit();
        glPopMatrix();

        glColor3f(1, 1, 1);
        ofSetLineWidth(2);
        wireframe();
        glPushMatrix();
        glTranslatef(navePos.x, navePos.y, navePos.z);
        glScalef(naveSizeX * 1.3, naveSizeY * 1.3, naveSizeZ * 1.3);
        cube_malha_unit(1, 1);
        glPopMatrix();
        ofSetLineWidth(1);

        if (waveCounter == 1) {
            for (int i = 0; i < wave1.size(); i++) {
                wave1[i].draw();
            }
        }
        else {
            for (int i = 0; i < wave2.size(); i++) {
                wave2[i].draw();
            }
        }

        for (int i = 0; i < bulletsMidAir.size(); i++) {
            bulletsMidAir[i].draw();
        }

    }
    else if (state == 2) setupGLWorld();
}

// comandos que cada tecla realiza, transitar entre movimentos, mudar parâmetros, reset de algumas variáveis e checks das mesmas
void Game::keyPressed(int key)
{
    switch (key)
    {

    case OF_KEY_LEFT:
    { 
        if (!isMoving) {
            if (gridMinX < navePos.x && camState < 2) {
                moveDir = -1;
                isMoving = true;
            }
            else if (gridMaxX > navePos.x && camState == 2) {
                moveDir = 1;
                isMoving = true;
            }
        }
        break;
    }
    case OF_KEY_RIGHT:
    { 
        if (!isMoving) {
            if (gridMaxX > navePos.x && camState < 2) {
                moveDir = 1;
                isMoving = true;
            }
            else if (gridMinX < navePos.x && camState == 2){
                moveDir = -1;
                isMoving = true;
            }
        }
        break;
    }
    case OF_KEY_UP:
    {
        if (isStartingScreen) {
            state++;
            resetPhysics();
            isStartingScreen = false;
        }
        if (state == 2) {
            state++;
            isStartingScreen = true;
            
        }
        break;
    }
    case ' ':
    {   
        
        if (canShoot && !isShooting) isShooting = true;
        break;
    }
    case 'c':
        camState++;
        printf("CamState Changed %d\n", camState);
        break;
    }

    if (state > numStates) state = 0;
    if (camState >= numCamStates) camState = 0;

}

void Game::keyReleased(int key)
{
}

void Game::moveNaveHelper(ofVec3f finalPos){

    float dist = finalPos.x - navePos.x;

    navePos.x += dist * naveEasing;
    if (navePos.squareDistance(finalPos) <= 1) {
        navePos.x = target.x;
        isMoving = false;
    }

}

void Game::moveNave() {

    if (isMoving) {
        target = (moveDir == 1) ? targetRPos : targetLPos;
        moveNaveHelper(target);
    }
    if (navePos == targetRPos) {
        targetRPos.x += gridSizeX;
        targetLPos.x += gridSizeX;
    }
    else if (navePos == targetLPos) {
        targetRPos.x -= gridSizeX;
        targetLPos.x -= gridSizeX;
    }

}

void Game::shoot() {

    if (canShoot && isShooting) {
        printf("Shoot\n");
        Bullet aux;
        aux.setup(navePos.x, navePos.y, navePos.z, shootSize, shootSize, shootSize,shootVel);
        bulletsMidAir.push_back(aux);
        isShooting = false;
        canShoot = false;
        shootFrameCounter = 0;
    }
    

}

void Game::updateBullets() {
    
    for (int i = 0; i < bulletsMidAir.size(); i++) {
        bulletsMidAir[i].update();
        if (bulletsMidAir[i].pos.y > 6 * gridSizeY || bulletsMidAir[i].hasHit) bulletsMidAir.erase(bulletsMidAir.begin() + i); // deixar de carregar as balas que sairam do campo de visão ou que já atingiram um target
    }

}

void Game::checkCollisions(vector<Enemy> &wave)
{
    /*std::vector<int> bulletsToRemove;
    std::vector<int> enemiesToRemove;

    for (int i = 0; i < bulletsMidAir.size(); i++) {
        shootPos = bulletsMidAir[i].pos;
        for (int j = 0; j < wave.size(); j++) {
            enemyPos = wave[j].pos;
            bool touching = false;

            if (shootPos.y + shootSize * 0.5 >= enemyPos.y - wave[j].dimensions.y * 0.5 &&
                shootPos.y + shootSize * 0.5 < enemyPos.y + wave[j].dimensions.y * 0.5 - shootSize) {
                if (shootPos.x - shootSize * 0.5 <= enemyPos.x + wave[j].dimensions.x * 0.5 &&
                    shootPos.x - shootSize * 0.5 > enemyPos.x - wave[j].dimensions.x * 0.5 - shootSize) {
                    touching = true;
                }
            }

            if (touching) {
                bulletsToRemove.push_back(i);
                wave[j].life--;
                if (wave[j].life <= 0) enemiesToRemove.push_back(j);
            }
        }
    }

    // Remover balas e inimigos marcados
    for (int i = bulletsToRemove.size() - 1; i >= 0; i--) {
        bulletsMidAir.erase(bulletsMidAir.begin() + bulletsToRemove[i]);
    }
    for (int i = enemiesToRemove.size() - 1; i >= 0; i--) {
        wave.erase(wave.begin() + enemiesToRemove[i]);
    }*/
    for (int i = 0; i < bulletsMidAir.size(); i++) {
        shootPos = bulletsMidAir[i].pos;
        for (int j = 0; j < wave.size(); j++) {
            enemyPos = wave[j].pos;
            bool touching = false;

            if (shootPos.y + shootSize * 0.5 >= enemyPos.y - wave[j].dimensions.y * 0.5 && 
                shootPos.y + shootSize * 0.5 < enemyPos.y + wave[j].dimensions.y * 0.5 - shootSize) // basicamente, se a bala está dentro do enemy, segundo o eixo y
            {
                if (shootPos.x - shootSize * 0.5 <= enemyPos.x + wave[j].dimensions.x * 0.5 && 
                    shootPos.x - shootSize * 0.5 > enemyPos.x - wave[j].dimensions.x * 0.5 - shootSize) 
                {
                    touching = true;
                }
            }

            if (touching) {
                printf("%d\n",touching);
                wave[j].life--;
                wave[j].update(ofVec3f(0, 0, 0));
                bulletsMidAir[i].hasHit = true;
                bulletsMidAir[i].update();
                

            }
            if (wave[j].dead == true) {
                wave.erase(wave.begin() + j);
                points += 10;
            }
        }
        if (bulletsMidAir[i].hasHit) {
            bulletsMidAir.erase(bulletsMidAir.begin() + i);
        }
    }
}

void Game::checkLoss() {
    if (waveCounter == 1) checkLossHelper(wave1);
    else checkLossHelper(wave2);
}

void Game::checkLossHelper(vector<Enemy>& wave)
{
    for (int i = 0; i < wave.size(); i++) {
        if (wave[i].pos.y < navePos.y) {
            winState = false;
        }
    }
    if (!winState) state++;
}

void Game::updateCam() 
{

    switch (camState) {
    case 0: // Orthographic View
    {
        glTranslatef(gw() * .5, gh() * .5, 0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-gw() * 0.5, gw() * 0.5, -gh() * 0.5, gh() * 0.5, -10000, 10000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        break;
    }
    case 1: // First-Person View
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        perspective(60, gw() / gh(), 0.1, 10000.0); // FOV ajustado para visão em primeira pessoa

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        lookat(navePos.x, navePos.y, navePos.z,navePos.x, navePos.y +gridSizeY,navePos.z + 10,0, 1, 0);                                        
        break;
    }

    case 2: // "2.5D" Perspective View
    {   // teste de profundidade feito pelo chatGPT, projeção da câmera feita á mão por tentativa e erro
        glEnable(GL_DEPTH_TEST); // Ativa o teste de profundidade
        glDepthFunc(GL_LEQUAL);  // Define a função de teste

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o buffer de cor e profundidade

        // Configura projeção e câmera
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        perspective(20, gw() / gh(), 0.1, gridSizeX * 12); // Projeção perspectiva

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        lookat(-gridSizeX*3, - gridSizeY * 16, 1000,0, 0, 0, 0, 1, 0);
        break;
    }
    }
}

void Game::makeWave1() {
    
    wave1.clear(); // caso precise de resetar a meio do "nível" porque o player perdeu uma vida

    for (int i = 0; i < 8; i++) {
        if (i % 2 == 0) {
            Enemy aux;
            aux.setup((-3 + i) * gridSizeX, 3 * gridSizeY, 0, naveSizeX, naveSizeY, naveSizeZ);
            wave1.push_back(aux);
          
        }
    }
    for (int i = 0; i < 7; i++) {
        Enemy aux;
        aux.setup((-3 + i) * gridSizeX, 4 * gridSizeY, 0, naveSizeX, naveSizeY, naveSizeZ);
        wave1.push_back(aux);
        
    }

}
// still needs more enemies here 
void Game::makeWave2() {

    wave2.clear(); // caso precise de resetar a meio do "nível" porque o player perdeu uma vida

    for (int i = 0; i < 8; i++) {
        if (i % 2 == 0) {
            Enemy aux;
            aux.setup((-3 + i) * gridSizeX, 3 * gridSizeY, 0, naveSizeX, naveSizeY, naveSizeZ);
            wave2.push_back(aux);

        }
    }
    for (int i = 0; i < 7; i++) {
        Enemy aux;
        aux.setup((-3 + i) * gridSizeX, 4 * gridSizeY, 0, naveSizeX, naveSizeY, naveSizeZ);
        wave2.push_back(aux);

    }

}

void Game::moveEnemies() {
    
    if (waveCounter == 1) {
        for (int i = 0; i < wave1.size(); i++) {
            wave1[i].update(ofVec3f(0, -gridSizeY, 0));
        }
    }
    else {
        for (int i = 0; i < wave2.size(); i++) {
            wave2[i].update(ofVec3f(0, -gridSizeY, 0));
        }
    }

}