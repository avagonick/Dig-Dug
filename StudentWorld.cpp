#include "StudentWorld.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
 int StudentWorld::init()
{
    //initializes the Earth
  addEarth();
  
    //initializes the tunnelman
    tunnel = new TunnelMan(this);
    
    //sets seeds for randomness so can have random placenment of objects
    srand((unsigned) time(NULL));
    
    //set boulders
    setBoulders();
    
    //set Barrels
    setBarrels();
    
    //addNuggets
    addNuggets();
     
     //get correct Protester info
     initProtesters();
     
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    ticks++; //increase the number of ticks each time this is called
    //set the game display text
    setGameStatText(setDisplayText());
    
    //add goodies that can be added this tick
   addGoodie();
    
    //allows the tunnel to do something
    tunnel -> doSomething();
    
    //addNewprotesterIf Should Thi sincludes the very first one on the first tick
    addNewProtesterIfShould();
    
    //if number of barrels left is 0 then play finished level sound and return
    if(numBarrelsLeft == 0){
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
 //go through all objects calling their do something function
   for(int i = 0; i < objects.size(); i++){
        objects[i] -> doSomething();
        
        //if TunnelMan has 0 hit points he died
       if(tunnel->getHitPoints() == 0){
           decLives();
               return GWSTATUS_PLAYER_DIED;

               }
    }
    
    
return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //delete all the dynamically allocated Earth
    deleteEarth();
    
    //delete dead objects
    for(int i = 0; i < objects.size(); i++){
        if(!objects[i] -> getAlive()){
            delete objects[i];
            objects.erase(objects.begin() + i);
        }
    }    
    
    //delete dynamically allocated tunnelman
    delete tunnel;
    
    //delete all of the objects
    for(int i = 0; i < objects.size(); i++){
        delete objects[i];
    }
    objects.clear();
}

//gives the earths array
Earth* StudentWorld::getearths(int i, int j ) const{
    
    return earths[i][j];
}

vector<actor*> StudentWorld::getObjects() const{
    return objects;
}

int StudentWorld::getTicks() const{
    return ticks;
}
//return number of barrels
int StudentWorld::numBarrels() const{
    return numBarrelsLeft;
}

void StudentWorld::changeBarrels() {
    numBarrelsLeft --;
}

TunnelMan* StudentWorld::getTunnelMan(){
    return tunnel;
}

int StudentWorld::TunnelManXpos() const{
    return tunnel -> getX();
}

int StudentWorld::TunnelManYpos() const{
    return tunnel -> getY();
}
void StudentWorld::decreaseNumberProtesters(){
    numberProtesters --;
}

void StudentWorld::removeSonar(){
    sonarInField = false;
}

//Protester stuff
int StudentWorld::getTargetProtesters(){
    double mins = 2 + getLevel() * 1.5;
   if( mins < 15)
       return mins;
    else
        return 15;
}


//tells you if you have icorrect overlap with boulder from what was said on piazza
bool StudentWorld::isNoBadOverLapBoulder(int x, int y) const{
    //must loop through all possible objects
    for(int i = 0; i < objects.size(); i++){
        
        //if find a boulder then check for overlap issues
        if(objects[i]->getType() == "Boulder" && objects[i]->getAlive() == true){
            //must check everything but the four corners
            //check if issues with overlapping with the passed objeect
            for(int x1 = x; x1 <= x + 3; x1++)
                for(int y1 = y; y1 <= y + 3; y1++){
                    //check overlap bottom edge
                    if((x1 == objects[i]->getX() + 1 && y1 == objects[i]->getY()) || (x1 == objects[i]->getX() + 2 && y1 == objects[i]->getY())){
                        //then have an overlap
                        return false;
                    }
                    //check overlap top edge
                    if((x1 == objects[i]->getX() + 1 && y1 == objects[i]->getY() + 3) || (x1 == objects[i]->getX() + 2 && y1 == objects[i]->getY())){
                        //then have an overlap
                        return false;
                    }
                    //no overlap left edge
                    if((x1 == objects[i]->getX() && y1 == objects[i]->getY() + 1) || (x1 == objects[i]->getX() && y1 == objects[i]->getY() + 2)){
                        //then have an overlap
                        return false;
                    }
                    //no over lap right edge
                    if((x1 == objects[i]-> getX() + 3 && y1 == objects[i]-> getY() + 1) || (x1 == objects[i]->getX() + 3 && y1 == objects[i]->getY() + 2)){
                        //then have an overlap
                        return false;
                    }
                    //make sore no overlap in center
                    for(int checky = objects[i]-> getY() + 1; checky < objects[i]-> getY() + 3; checky++){
                        for(int checkx = objects[i]-> getX() + 1; checkx < objects[i]-> getX() + 3; checkx++){
                            if(checky == y1 && checkx == x1)
                                return false;
                        }
                    }
                }
        }
    }
    return true;
}


//get Euclidean distance
double StudentWorld::getEuclidDist(double x1, double y1, double x2, double y2) const{
    int xs =pow(x2-x1, 2);
    int ys = pow(y2-y1, 2);
    return sqrt(xs + ys);
}

//tells you is there is any Earth overlap
bool StudentWorld::isEarthHere(int x, int y) const{
    for(int i = x; i <= x+ 3; i++){
        for(int j = y; j <= y + 3; j++){
            if( earths[i][j]->isVisible())
                return true;
        }
    }
    return false;
}

//tells you if valid coordinates
bool StudentWorld::isValidCoords(int x, int y) const{
    if (x < 0 || x > 60 )
        return false;
    if( y < 0 || y > 60)
        return false;

    return true;
}

void StudentWorld::isRad3Protest(actor * act) const{
    //must loop through all possible objects
    for(int i = 0; i < objects.size(); i++){
        //if find a protester or regular protester that is alive
        if((objects[i]->getType() == "regularProtester" || objects[i]->getType() == "hardCoreProtester") && objects[i]->getAlive() == true){
            //if less than a euclidian distance of three of the actor
            if(getEuclidDist(objects[i]->getX(), objects[i]->getY(), act->getX(), act->getY()) <= 3)
                //then iteract with the protester the way that specific actor should
                act->interactWithProtester(objects[i]);
        }
    }
}


void StudentWorld::addEarth()
{
    //set everything to Earth
    for(int i = 0; i <= 63; i++){
        for(int j = 0; j <= 63; j++){
            earths[i][j] = new Earth(i, j, this);
        }
    }
    
    //set what is left to be not visible
    for(int i = 30; i <= 33; i++){
        for(int j = 4; j <= 59; j++)
            earths[i][j]->setVisible(false);
    }
    
    for(int i = 0; i <= 63; i++){
        for(int j = 60; j<= 63; j++){
            earths[i][j]->setVisible(false);
        }
    }
}

//set the Boulders
void StudentWorld::setBoulders(){
    int bottomcheck = getLevel()/2+2;
    int numBoulders = min(bottomcheck, 9);
    
    for(int i = 0; i < numBoulders;){
        int randomx = rand() % 61;
        int randomy = 20 + rand() % 37;
        //not 29 or 34 because can't be 1 away from the shaft
        if (isGoodSpot(randomx, randomy))
        {
            objects.push_back(new Boulders(randomx, randomy, this));
            i++;
            delete4x4(randomx,randomy);
        }
    }
}


void StudentWorld::setBarrels(){
    int bottomcheck = getLevel()+2;
    numBarrelsLeft = min(bottomcheck, 21);
    for(int i = 0; i < numBarrelsLeft;){
        int randomx = rand() % 61;
        int randomy = rand() % 57;
        if (isGoodSpot(randomx, randomy))
        {
            objects.push_back(new Barrel(randomx, randomy, this));
            i++;
        }
    }
}


void StudentWorld::addNuggets(){
    int numGoldmax = 5 - getLevel()/2;
    int numGold = max(numGoldmax, 2);
    
    for(int i = 0; i < numGold;){
        int randomx = rand() % 61;
        int randomy = rand() % 57;
        if (isGoodSpot(randomx, randomy))
        {
            objects.push_back(new GoldNugget(randomx, randomy, this, false, "Permanent", "TunnelMan"));
            i++;
        }
    }
}



bool StudentWorld::isNoObjectOverlap(int x, int y) const{
    //check for Euclidean overlap, if there is an overlap return false
    for(int i = 0; i < objects.size();i++){
        if(getEuclidDist(x, y, objects[i]->getX(), objects[i]->getY()) <= 6)
            return false;
    }
    return true;
}

//checks if this is an ok place to be placed
bool StudentWorld::isGoodSpot(int x, int y) const{
    
    //check for Euclidean overlap, if there is an overlap return false
  if(!isNoObjectOverlap(x, y))
      return false; 
    
    //make sure that it is placed in Earth if returns false means it is placed in the tunnel
    for(int x1 = x; x1 <= x+3; x1++){
        for(int y1 = y; y1<= y+3; y1++){
            if( x1>= 30 && x1 <= 33 && y1>= 4 && y1<= 59)
                return false;
        }
    }
    if(x == 26 || x == 34 || x == 0|| x == 60 || y == 0|| y == 59)
        //cant be next to the shaft or on any of the four edges
        return false;
    

    return true;
}

//delete 4 by 4 area
void StudentWorld::delete4x4(int x, int y) {
    for(int x1 = x; x1 <= x+3; x1++){
        for(int y1 = y; y1 <= y + 3; y1++){
            if(x1 >= 0 && x1 <= 63 && y >= 0 && y <= 59)
            earths[x1][y1]->setVisible(false);
            
        }
    }
}


void StudentWorld::addGoldNugget(){
    objects.push_back(new GoldNugget(TunnelManXpos(), TunnelManYpos(), this, true, "Temporary", "Protesters"));
}

//this adds a squirt at the position told 
void StudentWorld::addSquirt(int x, int y){
    //only create a new object can move to the spot there is nothing there and it is a valid position
    if(x >= 0 && x <= 60 && y >= 0 && y <= 60  && !isEarthHere(x, y) && isNoBadOverLapBoulder(x,y))
        objects.push_back(new Squirt(x, y, this, tunnel ->getDirection()));
    playSound(SOUND_PLAYER_SQUIRT);
}

void StudentWorld::addGoodie(){
//randomness time add sonar
    
    //add a new goodie
    int G = getLevel() * 25 + 300;

    //happens one in G times
    if(rand() % G == 0){
        //one if 5 chance get sonar
        if(rand() % 5 == 0){
            //if already have a sonar in the field return
            if(sonarInField == true)
                return;
            objects.push_back(new SonarKit(this));
            sonarInField = true;
        }
        else
            //the other 4/5 times you create a new water pool at a random open position
            createWaterPool();
    }
}

//this adds a new water pool at a random spot 
void StudentWorld::createWaterPool(){
    int randomx;
    int randomy;
    do{
        randomx = rand() % 61;
        randomy = rand() % 57;
       
        //keep going through till get spot with no earth
    } while(isEarthHere(randomx, randomy));
       objects.push_back(new WaterPool(randomx, randomy, this));
}


//for now just adds a regular protester this should be in init
void StudentWorld::addOrigProtester(){
    ticksSinceLastProtester = 0;
    numberProtesters = 1;
    objects.push_back(new regularProtester(this));
}

int StudentWorld::tickSoCanAddNewProtest() const{
    int maxs = 200-getLevel();
    return max(25, maxs);
}

//currently everything is set up for regular protesters
void StudentWorld::addNewProtesterIfShould(){
    //first increase number of ticks that have passed
    ticksSinceLastProtester++;
    //if time to add a new Protester
    if(ticksSinceLastProtester >= tickSoCanAddNewProtest() && numberProtesters != MaxNumProtest){
        //if correct percentage of adding a hardcare
        if(shouldAddHardCore()){
            objects.push_back(new hardCoreProtester(this));
        }
        else
            //else make a regular Protester 
            objects.push_back(new regularProtester(this));
        //set ticks since last added a protester to 0
        ticksSinceLastProtester = 0;
        //set the number of protesters to increase
        numberProtesters++;
    }
}

//these specs also ensure that during the first tick a protester is added
void StudentWorld::initProtesters(){
    ticksSinceLastProtester = tickSoCanAddNewProtest() - 1;
    numberProtesters = 0;
    MaxNumProtest = getTargetProtesters();
}

bool StudentWorld::shouldAddHardCore() const{
    int mins = getLevel()*10 + 30;
    int probabilityOfHardcore;
    if(mins < 90)
        probabilityOfHardcore = mins;
    else
        probabilityOfHardcore = 90;
    
    //probabilityOfHareCoretime return true 
    int number = rand() % 100 + 1;
    if(number <= probabilityOfHardcore)
        return true;
    else
        return false;
}

string StudentWorld::setDisplayText(){
    int level = getLevel();
    int lives = getLives();
    int health = tunnel->getHitPoints(); //this is hitpoints must convert this into a percetnage
    int squirts = tunnel->getWaterSquirt();
    int gold = tunnel->getGoldNuggets();
    int barrelsLeft = numBarrelsLeft;
    int sonar = tunnel ->getSonarCharges();
    int score = getScore();
    
    ostringstream display;
    
    //set the level with 2 digits
    display << "Lvl: " << setw(2) <<  level;
    
    //set lives next with 1 digit
    display << "  Lives: " << setw(1) << lives;
    
    //sethealth
    display << "  Hlth: " << setw(3) <<( health * 10) << "%";
    
    //setwater
    display << "  Wtr: "<< setw(2) << squirts;
    
    //set gold
    display << "  Gld: " << setw(2) << gold;
    
    //set oil
    display << "  Oil Left: " << setw(2) << barrelsLeft;
    
    //set sonary
    display << "  Sonar: "<< setw(2) << sonar;
    
    //set score
    display.fill('0');
    display << " Scr: "<< setw(6) << score;
    
    //change this into a string
    string disp = display.str();
    return disp;
}

//deletes Earth in the student world
void StudentWorld::deleteEarth(){
    for(int i = 0; i <= 63; i++){
        for(int j = 0; j  <= 59; j++)
            delete earths[i][j];
    }
    
}

//destructor at this current moment it doesn't actually do anything different
StudentWorld::~StudentWorld(){
}
