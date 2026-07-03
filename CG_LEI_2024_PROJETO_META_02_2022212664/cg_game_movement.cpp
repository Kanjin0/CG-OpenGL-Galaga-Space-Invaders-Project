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


    //Configuração das Luzes
    useLights = true;
    dirLightOn = false;
    pointLightLOn = false;
    pointLightROn = false;
    lanternOn = true;

    dirLightAmbOn = true;
    dirLightDifOn = true;
    dirLightSpecOn = true;

    pointLightAmbOn = true;
    pointLightDifOn = true;
    pointLightSpecOn = true;

    lanternAmbOn = true;
    lanternDifOn = true;
    lanternSpecOn = true;

    // Configuração inicial da luz de foco
    lanternCutoff = 11.0f; // Ângulo do cone em graus
    lanternExponent = 8; // Concentração da luz
    lanternTurnOnCounter = 0;
    lanternGoOffCounter = 0;
    

    //Posição da lanterna da nave
    lanternPos[0] = gw() * 0.5;
    lanternPos[1] = 12.5 * gridSizeY;
    lanternPos[2] = 30;
    lanternPos[3] = 1.0f;

    //Direção para onde aponta
    lanternDir[0] = 0.f;
    lanternDir[1] = -.6f;
    lanternDir[2] = 2.1;

    //Valores Ambiente de Lantern

    lanternAmb[0] = 3;
    lanternAmb[1] = 3; //ou tudo green ou talvez um ciano, já vejo
    lanternAmb[2] = 3;
    lanternAmb[3] = 1.0f;

    //Valores Difuse de Lantern

    lanternDif[0] = 1;   // Intensidade difusa mais alta
    lanternDif[1] = 0;
    lanternDif[2] = 0;
    lanternDif[3] = 1.0f;

    //Valores Specular de Lantern
    lanternSpec[0] = 1;  // Intensidade especular mais alta
    lanternSpec[1] = 1;
    lanternSpec[2] = 1;
    lanternSpec[3] = 1.0f;


    //Posição directional Light

    dirVec[0] = gw() * .5;
    dirVec[1] = 2 * gridSizeY;
    dirVec[2] = 30;
    dirVec[3] = 0;

    dirAmb[0] = 01.5;//R
    dirAmb[1] = 01.5;//G
    dirAmb[2] = 01.5;//B
    dirAmb[3] = 1.;//constante
    //o quanto vai variar a partir do valor base acima, numericamente correspondente, ou seja, se acima tiver no R 0.1 e aqui 1, ele não vai variar de 0.1 a 1
    dirDif[0] = 0.8;//R
    dirDif[1] = 0.5;//G
    dirDif[2] = 0.5;//B
    dirDif[3] = 1.;//constante
    // o quanto de luz que se espalha a partir da origem da luz, ou seja, a de cima, faz como se fosse raios que se espalham mas a intesidade é a mesma em todo o lugar, aqui varia com a distância
    dirSpec[0] = 0.1;//R
    dirSpec[1] = 0.1;//G
    dirSpec[2] = 0.1;//B
    dirSpec[3] = 1.;//constante


    //Posição luzes point

    pointPosL[0] = gw()*0.2;
    pointPosL[1] = gh() * 0.5;
    pointPosL[2] = 20;
    pointPosL[3] = 1;


    pointPosR[0] = gw() * 0.8;
    pointPosR[1] = gh() * 0.5;
    pointPosR[2] = 20;
    pointPosR[3] = 1;

    pointAmbL[0] = .0;//R
    pointAmbL[1] = 2.5;//G
    pointAmbL[2] = 2.5;//B
    pointAmbL[3] = 1.;//constante

    pointAmbL[0] = 2.5;//R
    pointAmbL[1] = 0;//G
    pointAmbL[2] = 2.5;//B
    pointAmbL[3] = 1.;//constante

    //valor maximo a que podem chegar quando perto da luz
    pointDifR[0] = 2.;//R
    pointDifR[1] = 0.1;//G
    pointDifR[2] = 0.1;//B
    pointDifR[3] = 1.;//

    pointDifL[0] = 0.1;//R
    pointDifL[1] = 2;//G
    pointDifL[2] = 0.1;//B
    pointDifL[3] = 1.;//constante

    pointSpec[0] = 0.1;//R
    pointSpec[1] = 0.1;//G
    pointSpec[2] = 0.1;//B
    pointSpec[3] = 1.;//constante

    hardMode = false;
    
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
    useLights = true;
    hardMode = false;
    
    // lookat(0, 0, 1, 0, 0, 0, 0, 1, 0);
}


// neste apenas estão as funçoes de cada tipo de movimento
void Game::update()
{
    if (state == 1 && winState)
    {
        updateLights();
        shootFrameCounter++;
        if (lanternOn) lanternGoOffCounter++;
        if (lanternGoOffCounter >= 260) {
            lanternTurnOnCounter = 0;
            lanternGoOffCounter = 0;
            lanternOn = false;
        }
        if (lanternTurnOnCounter > 10) {
            lanternOn = true;
            lanternTurnOnCounter = 0;
        }
        if (shootFrameCounter >= 12) canShoot = true;
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
        if (wave1.empty() && wave2.empty()) {
            glDisable(GL_LIGHTING);
            glDisable(GL_NORMALIZE);
            useLights = false;
            state++;
        }
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
        ofDrawBitmapString("WELCOME TO THE GAME\nPRESS UP TO START\nPRESS DOWN AND THEN UP TO PLAY ON HARDMODE", (gw() - 40 * 8) / 2, (gh() - 24) / 2);
        break;
    case 1:
        ofSetColor(255, 0, 0);
        if (hardMode) ofDrawBitmapString("PONTUACAO = " + ofToString(points*1.2), 30, 30);
        else ofDrawBitmapString("PONTUACAO = " + ofToString(points), 30, 30);
        
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
        if (useLights) {
            glEnable(GL_LIGHTING);
            glEnable(GL_NORMALIZE);
        }
        // Configuração da luz de foco
        if (lanternOn) {
            glLightfv(GL_LIGHT2, GL_POSITION, lanternPos); // Posição da luz
            glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lanternDir); // Direção da luz
            glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lanternCutoff); // Ângulo de corte
            glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lanternExponent); // Concentração

            glLightfv(GL_LIGHT2, GL_AMBIENT, lanternAmb);
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lanternDif);
            glLightfv(GL_LIGHT2, GL_SPECULAR, lanternSpec);

            glEnable(GL_LIGHT2);
            printf("lantern at %f,%f,%f\n",lanternPos[0], lanternPos[1], lanternPos[2]);
            printf("lantern at %f,%f,%f\n", navePos.x, navePos.y, navePos.z);
        }
        else {
            glDisable(GL_LIGHT2);
        }

        if (dirLightOn) {
            glLightfv(GL_LIGHT0, GL_POSITION, dirVec);
            glLightfv(GL_LIGHT0, GL_AMBIENT, dirAmb);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, dirDif);
            glLightfv(GL_LIGHT0, GL_SPECULAR, dirSpec);

            glEnable(GL_LIGHT0);
        }
        else {
            glDisable(GL_LIGHT0);
        }


        if (pointLightLOn) {
            glLightfv(GL_LIGHT1, GL_POSITION, pointPosL);
            glLightfv(GL_LIGHT1, GL_AMBIENT, pointAmbL);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, pointDifL);
            glLightfv(GL_LIGHT1, GL_SPECULAR, pointSpec);

            glEnable(GL_LIGHT1);

            ofSetColor(255, 255, 0);
            ofDrawSphere(pointPosL[0], pointPosL[1], pointPosL[2], naveSizeX);
        }
        else {
            glDisable(GL_LIGHT1);
        }

        if (pointLightROn) {
            glLightfv(GL_LIGHT3, GL_POSITION, pointPosR);
            glLightfv(GL_LIGHT3, GL_AMBIENT, pointAmbR);
            glLightfv(GL_LIGHT3, GL_DIFFUSE, pointDifR);
            glLightfv(GL_LIGHT3, GL_SPECULAR, pointSpec);

            glEnable(GL_LIGHT3);

            ofSetColor(255, 0, 255);
            ofDrawSphere(pointPosR[0], pointPosR[1], pointPosR[2], naveSizeX);
        }
        else {
            glDisable(GL_LIGHT3);
        }


        updateCam();

        glColor3f(1., 1., 1);
        wireframe();
        glPushMatrix();
        glTranslatef(0, 0, 0);
        glScalef(entityScalingX, entityScalingY, entityScalingZ);
        malha_unit(gridRows, gridCols);
        glPopMatrix();

        loadMaterial(0);
        filled();
        glPushMatrix();
        glTranslatef(navePos.x, navePos.y, navePos.z);
        glScalef(naveSizeX, naveSizeY, naveSizeZ);
        cube_unit();
        glPopMatrix();

        loadMaterial(8);
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
            lanternTurnOnCounter = 0;
            lanternGoOffCounter = 0;
            lanternOn = true;
            
        }
        break;
    }

    case OF_KEY_DOWN:
    {
        if (isStartingScreen) {
            hardMode = true;
        }
        break;
    }

    case ' ':
    {   
        
        if (canShoot && !isShooting) isShooting = true;
        break;
    }
    case '1':
    {
        if (!hardMode) lanternOn = !lanternOn;
        else lanternOn = true;
        break;
    }
    case '2':
    {   
        if (!hardMode) dirLightOn = !dirLightOn;
        break;
    }
    case '3':
    {
        if (!hardMode) pointLightLOn = !pointLightLOn;
        break;
    }
    case '4':
    {
        if (!hardMode) pointLightROn = !pointLightROn;
        break;
    }
    case '5':
    {   
        if (lanternOn) {
            lanternAmbOn = !lanternAmbOn;
        }
        if (dirLightOn) {
            dirLightAmbOn = !dirLightAmbOn;
        }
        if (pointLightLOn || pointLightROn) {
            pointLightAmbOn = !pointLightAmbOn;
        }
        break;
    }
    case '6':
    {   
        if (lanternOn) {
            lanternDifOn = !lanternDifOn;
        }
        if (dirLightOn) {
            dirLightDifOn = !dirLightDifOn;
        }
        if (pointLightLOn || pointLightROn) {
            pointLightDifOn = !pointLightDifOn;
        }
        break;
    }
    case '7':
    {
        if (lanternOn) {
            lanternSpecOn = !lanternSpecOn;
        }
        if (dirLightOn) {
            dirLightSpecOn = !dirLightSpecOn;
        }
        if (pointLightLOn || pointLightROn) {
            pointLightSpecOn = !pointLightSpecOn;
        }
        break;
    }
    case 'q':
    {
        if (!lanternOn) lanternTurnOnCounter++;
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
        lanternPos[0] += gridSizeX;
    }
    else if (navePos == targetLPos) {
        targetRPos.x -= gridSizeX;
        targetLPos.x -= gridSizeX;
        lanternPos[0] -= gridSizeX;
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
            aux.setup((-3 + i) * gridSizeX, 2 * gridSizeY, 0, naveSizeX, naveSizeY, naveSizeZ);
            wave2.push_back(aux);

        }
    }
    for (int i = 0; i < 7; i++) {
        Enemy aux;
        aux.setup((-3 + i) * gridSizeX, 3 * gridSizeY, 0, naveSizeX, naveSizeY, naveSizeZ);
        wave2.push_back(aux);

    }
    for (int i = 0; i < 7; i++) {
        Enemy aux;
        aux.setup((-3 + i) * gridSizeX, 5 * gridSizeY, 0, naveSizeX, naveSizeY, naveSizeZ);
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

void Game::updateLights() {
    if (lanternOn && !hardMode) {
        if (lanternAmbOn) {
            lanternAmb[0] = 3;
            lanternAmb[1] = 3; //ou tudo green ou talvez um ciano, já vejo
            lanternAmb[2] = 3;
            lanternAmb[3] = 1.0f;
        }
        else {
            lanternAmb[0] = 0;
            lanternAmb[1] = 0; //ou tudo green ou talvez um ciano, já vejo
            lanternAmb[2] = 0;
            lanternAmb[3] = 1.0f;
        }
        if (lanternDifOn) {
            lanternDif[0] = 1;   // Intensidade difusa mais alta
            lanternDif[1] = 0;
            lanternDif[2] = 0;
            lanternDif[3] = 1.0f;
        }
        else {

            lanternDif[0] = 0;   // Intensidade difusa mais alta
            lanternDif[1] = 0;
            lanternDif[2] = 0;
            lanternDif[3] = 1.0f;

        }
        if (lanternSpecOn) {
            lanternSpec[0] = 1;  // Intensidade especular mais alta
            lanternSpec[1] = 1;
            lanternSpec[2] = 1;
            lanternSpec[3] = 1.0f;
        }
        else {
            lanternSpec[0] = 0;  // Intensidade especular mais alta
            lanternSpec[1] = 0;
            lanternSpec[2] = 0;
            lanternSpec[3] = 1.0f;
        }
    }
    if (dirLightOn) {
        if (dirLightAmbOn) {
            lanternAmb[0] = 3;
            lanternAmb[1] = 3; //ou tudo green ou talvez um ciano, já vejo
            lanternAmb[2] = 3;
            lanternAmb[3] = 1.0f;
        }
        else {
            lanternAmb[0] = 0;
            lanternAmb[1] = 0; //ou tudo green ou talvez um ciano, já vejo
            lanternAmb[2] = 0;
            lanternAmb[3] = 1.0f;
        }
        if (dirLightDifOn) {
            dirAmb[0] = 1.5;//R
            dirAmb[1] = 1.5;//G
            dirAmb[2] = 1.5;//B
            dirAmb[3] = 1.;//constante
        }
        else {
            dirAmb[0] = 0;//R
            dirAmb[1] = 0;//G
            dirAmb[2] = 0;//B
            dirAmb[3] = 1.;//constante
        }
        if (dirLightSpecOn) {
            dirSpec[0] = 0.1;//R
            dirSpec[1] = 0.1;//G
            dirSpec[2] = 0.1;//B
            dirSpec[3] = 1.;//constante
        }
        else {
            dirSpec[0] = 0.;//R
            dirSpec[1] = 0.;//G
            dirSpec[2] = 0.;//B
            dirSpec[3] = 1.;//constante
        }
    }
    if (pointLightLOn || pointLightROn) {
        if (pointLightAmbOn) {
            pointAmbL[0] = .0;//R
            pointAmbL[1] = 2.5;//G
            pointAmbL[2] = 2.5;//B
            pointAmbL[3] = 1.;//constante

            pointAmbR[0] = 2.5;//R
            pointAmbR[1] = 0;//G
            pointAmbR[2] = 2.5;//B
            pointAmbR[3] = 1.;//constante
        }
        else {
            pointAmbL[0] = .0;//R
            pointAmbL[1] = 0;//G
            pointAmbL[2] = 0;//B
            pointAmbL[3] = 1.;//constante

            pointAmbR[0] = 0;//R
            pointAmbR[1] = 0;//G
            pointAmbR[2] = 0;//B
            pointAmbR[3] = 1.;//constante


        }
        if (pointLightDifOn) {
            pointDifR[0] = 2.;//R
            pointDifR[1] = 0.1;//G
            pointDifR[2] = 0.1;//B
            pointDifR[3] = 1.;//

            pointDifL[0] = 0.1;//R
            pointDifL[1] = 2;//G
            pointDifL[2] = 0.1;//B
            pointDifL[3] = 1.;//constante
        }
        else {
            pointDifR[0] = 0.;//R
            pointDifR[1] = 0;//G
            pointDifR[2] = 0;//B
            pointDifR[3] = 1.;//

            pointDifL[0] = 0.;//R
            pointDifL[1] = 0;//G
            pointDifL[2] = 0;//B
            pointDifL[3] = 1.;//constante
        }
        if (pointLightAmbOn) {
            pointSpec[0] = 0.1;//R
            pointSpec[1] = 0.1;//G
            pointSpec[2] = 0.1;//B
            pointSpec[3] = 1.;//constante
        }
        else {
            pointSpec[0] = 0.;//R
            pointSpec[1] = 0.;//G
            pointSpec[2] = 0.;//B
            pointSpec[3] = 1.;//constante
        }
    }
}