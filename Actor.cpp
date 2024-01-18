#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
#include <algorithm>
#include <cstdlib>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


//ACTORS FUNCTIONS
//constructor
actor::actor( int imageID, int startX, int startY, Direction startDirection,double size, unsigned int depth, StudentWorld* studentworld, std::string types ):GraphObject(imageID, startX, startY, startDirection, size, depth){
    m_StudentWorld = studentworld;
    setVisible(true);
    //start by setting alive status to true
    m_isAlive = true;
    //set the type
    m_type = types;
}

//getters and setters
//getter to get the world in
StudentWorld* actor::getWorld() const{
    return m_StudentWorld;
}

//tells you alive status
bool actor::getAlive() const{
    return m_isAlive;
}

//sets alive status
void actor::setAlive(bool set){
    m_isAlive = set;
}

//gets type
std::string actor::getType() const{
    return m_type;
}


//checks if in the direction want to move there is a Boulder in the way
bool actor::isBoulderInDirBadOverLap(int x, int y, Direction dir) const{
    //checks each direction and returns true if there is any dirt there
    if(dir == right){
        //first make sure valid coordinates
        if(getWorld()->isValidCoords(x + 1, y))
            return(getWorld()->isNoBadOverLapBoulder(x + 1, y));
    }
    
    if(dir == left){
        if(getWorld()->isValidCoords(x - 1, y))
        return(getWorld()->isNoBadOverLapBoulder( x - 1, y));
    }
    
    if (dir == up){
        if(getWorld()->isValidCoords(x, y + 1))
        return(getWorld()->isNoBadOverLapBoulder(x, y + 1));
        
    }
    if(dir == down){
        if(getWorld()->isValidCoords(x, y - 1))
            return(getWorld()->isNoBadOverLapBoulder(x, y-1));
    }
    
    return false; 
}

//tells you if there Earth in a specific direction but only in the 4 positions in the front
bool actor::isEarthInDir(int x, int y, Direction dir) const{
 //goes through and checks if there is Earth in any of the directions
    if(dir == right){
        for(int i = 0; i <=3; i++){
            //isValid Coords checks based on left hand
            if( getWorld()->isValidCoords(x + 1, y + i) && getWorld()->getearths(x + 4, y + i)->isVisible())
                return true;
        }
    }
    
    if(dir == left){
        for(int i = 0; i <=3; i++){
            if( getWorld()->isValidCoords(x - 1, y + i) && getWorld()->getearths(x - 1, y + i)->isVisible())
                return true;
        }
    }
    
    if (dir == up){
        for(int i = 0; i <=3; i++){
            //because is Valid Coords only checks if valid position for the coord so must be 60 or less
            if(getWorld()->isValidCoords(x + i, y + 1) && getWorld()->getearths(x + i, y + 4) ->isVisible())
                return true;
        }
    }
    else if(dir == down)
        for(int i = 0; i <=3; i++){
            if(getWorld()->isValidCoords(x + i, y - 1) && getWorld()->getearths(x + i, y - 1) -> isVisible())
                return true;
        }
    return false;
}
    
bool actor::isLessRad3FromTunnel() const{
    return getWorld()->getEuclidDist(getX(), getY(), getWorld()->TunnelManXpos(), getWorld() -> TunnelManYpos()) <= 3;
}
    

//if can move in direction move one square in a direction
void actor::moveInDir(int x, int y, Direction dir){
    if(dir == right)
        moveTo(x + 1, y);
    if(dir == left)
        moveTo(x - 1, y);
    if(dir == up)
        moveTo( x, y+1);
    if(dir == down)
        moveTo( x, y - 1);
}

//says if within radius of 4 set vis to true and return true
bool actor::setVisIf4(){
    if(isVisible() == false && getWorld()->getEuclidDist(getX(), getY(), getWorld()->TunnelManXpos(), getWorld()->TunnelManYpos()) <= 4.0){
       setVisible(true); //make it visible
        return true;
    }
    return false;
}
    
//defining it here because it is a virtual function but here it does nothing
//functions here just because they are virtual and are needed
void actor::interactWithProtester(actor* act){}
bool actor::decreaseHitPointsSoundIfAnnoyed(int decrease){return true;}
void actor::squirt(){}
void actor::getGold(){}
void actor::checkIfPicked(){}
void actor::doSomething(){}

//destructor
actor::~actor(){};

//EARTH FUNCTIONS
//constructor
Earth::Earth(int startX, int startY, StudentWorld* studentworld): actor(TID_EARTH, startX, startY, right, 0.25, 3, studentworld, "Earth"){};


//destructor
Earth::~Earth(){};

//PEOPLE FUNCTIONS
//constructor
people::people(int imageID, int startX, int startY, Direction startDirection,  double size, unsigned int depth, int hitpoints, StudentWorld* studentworld, std::string type): actor( imageID, startX, startY,startDirection,  size,  depth, studentworld, type){
    m_hitpoints = hitpoints;
};

//return people number of hitpoints
int people::getHitPoints() const{
    return m_hitpoints;
}

//allows you to set HitPoint
void people::decreaseHitPoint(int decrease){
    m_hitpoints -= decrease;
}

//allows you to set HitPoint
void people::setHitPoint0(){
    m_hitpoints = 0;
}

//destructor
people::~people(){};


//TUNNELMAN FUNCTIONS
//constructor
TunnelMan::TunnelMan(StudentWorld* studentworld): people (TID_PLAYER, 30,  60, right,  1, 0,  10, studentworld, "TunnelMan"){
    //sets the initial member values (hitpoints is set in people because this happens in multiple people
    m_watersquirts = 5;
    m_sonarcharges = 1;
    m_goldnuggets = 0;
};

//gets water squirts
int TunnelMan::getWaterSquirt() const{
    return m_watersquirts;
}

//gets gold nuggets
int TunnelMan::getGoldNuggets() const{
    return m_goldnuggets;
}

//gets sonar
int TunnelMan::getSonarCharges() const{
    return m_sonarcharges;
}

//adds Gold when find
void TunnelMan::addGoldNuggets(){
    m_goldnuggets++;
}

//add two sonar charges to the tunnelMan
void TunnelMan::addSonarCharge(){
    m_sonarcharges+= 2;
}

//addwater
void TunnelMan::addWater(){
    m_watersquirts += 5;
}

void TunnelMan::decreaseHitPointsescp(){
    setHitPoint0();
}

//TunnelMans version of this function
bool TunnelMan::decreaseHitPointsSoundIfAnnoyed(int decrease){
    //if subtract amount of hit points makes it 0 or more just subtract out the hitpoints
    if(getHitPoints() - decrease >= 0)
        decreaseHitPoint(decrease);
    
    //if subtract out the hitpoints makes it less than 0 decrease the number of hit points
    else
        setHitPoint0();
    
    if(getHitPoints() == 0)
        GameController::getInstance().playSound(SOUND_PLAYER_GIVE_UP);
    //return true if does something
    return true;
}

void TunnelMan::digEarth(){
    bool didig = false;
    
    //for each direction only need to check the 4x1 block it just moved into because everything else has to be free
    
    if(getDirection() == right){
        //only check right side cause this is where move
        for(int y = getY(); y <= getY()+3; y++){
            if(getX() + 3 >= 0 && getX() + 3 <= 63 && y >= 0 && y <= 59)
                //if in the same position and there is Earth there (ie it is visible dig it up and turn it to be not visible
                if(getWorld()->getearths(getX() + 3,y)->isVisible()){
                    getWorld()->getearths(getX() + 3, y) -> setVisible(false);
                    didig = true;
                }
        }
    }
    
    if(getDirection() == left){
        //only check left side cause this is where move
        for(int y = getY(); y <= getY()+3; y++){
            if(getX() >= 0 && getX()<= 63 && y >= 0 && y <= 59)
                if(getWorld()->getearths(getX(),y)->isVisible()){
                    getWorld()->getearths(getX(), y) -> setVisible(false);
                    didig = true;
                }
        }
    }
    
    if(getDirection() == up){
        //only check upper side cause this is where move
        for(int x = getX(); x <= getX()+3; x++){
            if(getX()>= 0 && getX() <= 63 && getY() >= 0 && getY() <= 59)
                if(getWorld()->getearths(x, getY() + 3)->isVisible()){
                    getWorld()->getearths(x, getY() + 3) -> setVisible(false);
                    didig = true;
                }
        }
    }
    
    if(getDirection() == down){
        //only check down side cause this is where move
        for(int x = getX(); x <= getX()+3; x++){
            if(getX()>= 0 && getX() <= 63 && getY() >= 0 && getY() <= 59)
                //if in the same position and there is Earth there (ie it is visible dig it up and turn it to be not visible
                if(getWorld()->getearths(x, getY())->isVisible()){
                    getWorld()->getearths(x, getY()) -> setVisible(false);
                    didig = true;
                }
        }
    }
        

    if(didig == true){
        //if delete something play sounds
        GameController::getInstance().playSound(SOUND_DIG);
    }
}

//if you hit a key this tell soyu what to do 
void TunnelMan::ifKey(int key){
    //if press escape set alive to false
    if(key == KEY_PRESS_ESCAPE){
        //decrease hit points all the way
        decreaseHitPointsescp();
        return;
    }
    
    //if hit left and can move left move left
    if(key == KEY_PRESS_LEFT && getX() != 0 && getDirection() == left && isBoulderInDirBadOverLap(getX(), getY(), left )){
            moveTo(getX()-1, getY());
    }
        //if hit left but can only turn turn
    else if(key == KEY_PRESS_LEFT)
        setDirection(left);
    
    //if hit right and can move right move right
    else if(key == KEY_PRESS_RIGHT && getX() != 60 && getDirection() == right && isBoulderInDirBadOverLap(getX(), getY(), right)){
        moveTo(getX()+1, getY());
    //if hit right but can only turn turn
           }
    else if(key == KEY_PRESS_RIGHT)
        setDirection(right);
    
    //if hit up and can move up move
    else if(key == KEY_PRESS_UP && getY() != 60 && getDirection() == up && isBoulderInDirBadOverLap(getX(), getY(), up)){
        moveTo(getX(), getY() + 1);
        //if hit up and can only turn turn
    }
    else if(key == KEY_PRESS_UP)
        setDirection(up);
    
    //if hit move down and can move down
    else if(key == KEY_PRESS_DOWN && getY() != 0 && getDirection() == down && isBoulderInDirBadOverLap(getX(), getY(), down)){
        moveTo(getX(), getY() -1);
    }
    
    //if hit move down and can only turn turn
    else if(key == KEY_PRESS_DOWN)
        setDirection(down);
    
    //if press tab drop a gold nugget
    else if(key == KEY_PRESS_TAB)
        dropGoldNugget();
    
    //if press table add a new squirt
    else if(key == KEY_PRESS_SPACE){
        if(m_watersquirts == 0)
            return;
        m_watersquirts--;
        //add a squirt
        
        //make sure squirt is created in the right position
        if(getDirection() == right)
        getWorld()->addSquirt(getX() + 4, getY());
        
        if(getDirection() == left)
            getWorld()->addSquirt(getX()-4,getY());
        
        if(getDirection() == up)
            getWorld()->addSquirt(getX(),getY() + 4);
        
        if(getDirection() == down)
            getWorld()->addSquirt(getX(),getY() - 4);
    }
    
    //if press z do a sonar charge 
    else if(key == 'Z' || key == 'z'){
        doSonarCharge();
    }
}


void TunnelMan::dropGoldNugget(){
    if(m_goldnuggets == 0)
        return;
    else
        m_goldnuggets --;
    //add a gold nugget to the field that is visible and set to disappear after 30 ticks
    getWorld()->addGoldNugget();
}

void TunnelMan::doSonarCharge(){
    //return if no sonar charges left
    if(m_sonarcharges == 0)
        return;
    m_sonarcharges --;
    //play Sonar Charge sound
    GameController::getInstance().playSound(SOUND_SONAR);
    //set everything within a radius of 12 to true
    for(int i = 0; i < (getWorld()->getObjects()).size(); i++){
        if(getWorld() -> getEuclidDist(getX(), getY(), getWorld()->getObjects()[i]->getX(), getWorld()->getObjects()[i]->getY()) < 12 && getWorld()->getObjects()[i]->getAlive())
            getWorld()->getObjects()[i]->setVisible(true);
    }
}

//do something which relies on everything before it
//for now empty do something
void TunnelMan::doSomething(){
    //return if dead
    if(getHitPoints() == 0)
        return;
    //dig Earth that is touching
    digEarth();
    
    //if a key was touched on the keybaord
    int key;
    if(getWorld()->getKey(key)){
        //move if it a move involving move
        ifKey(key);
    }
    
    //moves then return if the number of barrels left is 0
    if(getWorld()->numBarrels() == 0)
        return;
};

//destructor
TunnelMan::~TunnelMan(){};

//OBJECTS functions
//constructor
Objects::Objects(int x, int y, StudentWorld* studentworld, std::string State, int TID, Direction direction, double size, unsigned int depth, std::string type): actor(TID, x, y, direction, size,depth, studentworld, type){
    m_state = State;
    m_ticksStartWaiting = 0;
}

//string getstate
std::string Objects::getState() const{
    return m_state;
}

//setsate
void Objects:: setState(std:: string set){
    m_state = set;
}

//get tickets waiting
int Objects:: getTicksWait() const{
    return m_ticksStartWaiting;
}

//set tickts waiting
void Objects::setTicksWait(int time){
    m_ticksStartWaiting = time;
}

void Objects::deleteIfTempTimeOver(){}

//destructor
Objects::~Objects(){}

//BOULDER FUNCTIONS
//constructor
Boulders::Boulders(int x, int y, StudentWorld* studentworld): Objects(x, y, studentworld, "stable", TID_BOULDER, down, 1, 1, "Boulder"){
    //set waiting ticks to 0 as default 
    setTicksWait(0);
}

//this is just to ensure boulders stop if falling on eachother, checks if Boulder below you
bool Boulders::checkBoulders(int x, int y) const{
    std::vector<actor*> obj = getWorld()->getObjects();
    //look through each object
        for(int i = 0; i < obj.size(); i++){
            if(obj[i]->getType() == "Boulder" && obj[i]->getAlive())
                //leave loop here becuase this means this is yourself
                if(obj[i]->getX() != x && obj[i]->getY() != y){
                    //check all x positions below it
                    for(int x1 = x; x1 <= x + 3; x1++){
                        //have to iterate to see if boulder there 
                        for(int bouldx = obj[i]->getX(); bouldx <= obj[i]->getX() + 3; bouldx++){
                            for(int bouldy = obj[i] -> getY(); bouldy <= obj[i]->getY() + 3; bouldy++)
                                //if any of the x positions with a y value of one below the boulder overlaps with the boulder
                                if(x1 == bouldx && y - 1 == bouldy)
                                    return true;
                        }
                    }
                }
        }
        return false;
}

//boulders can interact with protesters
void Boulders::interactWithProtester(actor* protest){
    //only increase score if not in the leave oil field state
    if(protest->decreaseHitPointsSoundIfAnnoyed(100)){
        getWorld()->increaseScore(500);
    }
}
void Boulders::doSomething(){
    //if not alive return
    if(!getAlive())
        return;
    
    //if stable check if blocks underneath if there is change to waiting
    if(getState() == "stable"){
        if(!isEarthInDir(getX(), getY(), down)){
            setState("waiting");
            setTicksWait(getWorld() -> getTicks());
        }
    }
    
    //do 30 ticks of waiting until change to falling
    else if(getWorld()-> getTicks()  - getTicksWait() == 30 && getState() == "waiting"){
        setState("falling");
        GameController::getInstance().playSound(SOUND_FALLING_ROCK);
    }
    
    //if falling fall until either hits grass or is at bottom and set alive to false 
   else if(getState() == "falling"){
       //if still no earth undreneath and no boulder underneath 
        if((!isEarthInDir(getX(), getY(), down) && getY() != 0 ) &&( !checkBoulders(getX(), getY()) && getY() != 0))
        {
            moveTo(getX(), getY() - 1);
            //if boulder gets within radius 3 of tunnel man tunnel man dies 
            if(isLessRad3FromTunnel()){
                //decrease hit points by 100
                getWorld()->getTunnelMan()->decreaseHitPointsSoundIfAnnoyed(100);
            }
            //check if it is within rad 3 of a protester if it is do the correct thing
            getWorld()->isRad3Protest(this);
            
        }
        else{
            setAlive(false);
            setVisible(false);
        }
    }
    
}

//destructor
Boulders::~Boulders(){}

//BARREL FUNCTIONS
//constructor 
Barrel::Barrel(int x, int y, StudentWorld* studentworld):actor(TID_BARREL, x, y, right, 1, 2, studentworld, "Barrel"){
    //keeps track of the fact that it starts off as not visible, and sets visiblility to false
    setVisible(false);
}

void Barrel::checkIfPicked(){
//if within a radius of 3 of TunnelMan
    if(isLessRad3FromTunnel()){
        GameController::getInstance().playSound(SOUND_FOUND_OIL);
        getWorld()-> increaseScore(1000);
        //decrease the number of Barrels by 1
        getWorld()->changeBarrels();
        //set alive to false
        setAlive(false);
        setVisible(false);
    }
}

void Barrel::doSomething(){
    //if not alive return
    if(!getAlive())
        return;
    
     //if within a radius of 4 set visible to true and then return
    if(setVisIf4()){
        return;
    }

    //if radius of 3 of tunnelman pick it up
    checkIfPicked();
}

//destructor
Barrel::~Barrel(){}


//GOLDNUGGET FUNCTIONS
//constructor
GoldNugget::GoldNugget(int x, int y, StudentWorld* studentworld, bool Visible, std::string states, std::string CanPickUp): Objects(x, y, studentworld, states, TID_GOLD, right, 1, 2, "GoldNugget"){
    
    //set visiblity because sometimes start visible and sometimes start not visible 
    setVisible(Visible);
    
    m_whoCanPickUp = CanPickUp; //set who can pick it up
    
    if(CanPickUp == "TunnelMan"){
        setTicksWait(0);
    }
    //if Protesters and temp state temp then set ticks since temp so that it can be kept track o f
    else if(CanPickUp == "Protesters" && states == "Temporary"){
        setTicksWait( getWorld()->getTicks());
    }
}

void GoldNugget::checkIfPicked(){
    //if within a radius of 3 of TunnelMan
    if(isLessRad3FromTunnel()){
        //play sound got goodie
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        //increase number of gold nuggets the tunnelman has
        getWorld()->getTunnelMan()->addGoldNuggets();
        getWorld()->increaseScore(10);
        //set alive to false
        setAlive(false);
        setVisible(false);
    }
}

void GoldNugget::deleteIfTempTimeOver(){
    //if in temporary state and more than 100 ticks have past set it to not alive
    if(getState() == "Temporary" && getWorld()->getTicks() - getTicksWait() == 100){
        setAlive(false);
        setVisible(false);
    }
}

void GoldNugget::interactWithProtester(actor* protest){
    //if interact with protester set alive and visible to false
    protest->getGold();
    setAlive(false);
    setVisible(false);
}

void GoldNugget::doSomething(){
    //if dead return
    if(!getAlive())
        return;
    
    //if in temp state and more than 100 ticks have passed delete
    deleteIfTempTimeOver();
    
    //if too much time passed then return and set dead
    if(!getAlive())
        return;
    
    //if not visible and within a radius of 4 of tunnel man set visible to true
    if(setVisIf4())
        return;
    
    if( m_whoCanPickUp == "TunnelMan")
        checkIfPicked();
    
    if(m_whoCanPickUp == "Protesters"){
        getWorld()->isRad3Protest(this);
    }
}
GoldNugget::~GoldNugget(){
}

//SQUIRT FUNCTIONS
Squirt::Squirt(int x, int y, StudentWorld* studentworld, Direction startDirection): Objects(x, y,studentworld, "First",TID_WATER_SPURT, startDirection, 1, 1, "Squirt"){
    squaresToTravel = 4;
}

//squirt interacts with a protester
void Squirt::interactWithProtester(actor* protest){
    //for squirts this decreases the hit points by two
    //also takes care of the fact that leave in oil state can't do this 
    if( protest->decreaseHitPointsSoundIfAnnoyed(2)){
        //squirt squits it ie stuns it if needed and adds the points if needed, but only stuns if didn't die because if die just want it to keep going
        protest->squirt();
    }
    //set alive to false after squirt something
    setAlive(false);
    setVisible(false);
}

void Squirt::doSomething(){
    //if dead return
    if(!getAlive())
        return;
    //don't run do something on conception because this is to many things at once
   if(getState() == "First"){
    setState("NotFirst");
     return;
  }
    //if within rad3 of a protester do the correct thing
    getWorld()->isRad3Protest(this);
    
    //return if hit a protester and died
    if(!getAlive())
        return;
    //check if can move in direction, so make sure there is no earth in the direction and there is no boulders
    if(!isEarthInDir(getX(), getY(), getDirection()) && isBoulderInDirBadOverLap(getX(), getY(), getDirection())){
        //then move in direction
        moveInDir(getX(),getY(), getDirection());
        //then decrease the number of squares to travel
        squaresToTravel--;
    }
    
    //if there is Earth in the direction or there is a boulder in the direction set alive to false so the objec can be destroyed
    else if (isEarthInDir(getX(), getY(), getDirection()) || !isBoulderInDirBadOverLap(getX(), getY(), getDirection())){
        setAlive(false);
        setVisible(false);
    }
    
    //if traveled all 4 squares it is dead
    if(squaresToTravel == 0){
        setAlive(false);
        setVisible(false);
    }
}


//squirt destructor
Squirt :: ~Squirt(){}
//SONAR KIT FUNCTIONS
//constructor
SonarKit::SonarKit( StudentWorld* studentworld): Objects(0, 60, studentworld, "temporary", TID_SONAR, right, 1, 2, "SonarKit"){
    //needs to be set since it can only last so long from creation 
    setTicksWait(getWorld()->getTicks());
}

void SonarKit::checkIfPicked(){
//if within a radius of 3 of TunnelMan
    if(isLessRad3FromTunnel()){
        //play sound got goodie
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        //increase number of Sonar Charges TunnelMan has
        getWorld()->getTunnelMan()->addSonarCharge();
        //increase the score
        getWorld()->increaseScore(75);
        //set alive to false
        setAlive(false);
        setVisible(false);
    }
}

void SonarKit::deleteIfTempTimeOver(){
    //find the max amount of ticks that it can be alive for
    int time = 300 - 10 * getWorld()->getLevel();
    int Tmax;
    if(time >= 100)
        Tmax = time;
    else
        Tmax = 100;
    
    //if hit the max amount of time it can get alive remove it
    if(getWorld()->getTicks() - getTicksWait() == Tmax){
        setAlive(false);
        setVisible(false);
        return;
    }
}

void SonarKit::doSomething(){
    //if not alive return
    if(!getAlive())
        return;
    deleteIfTempTimeOver();
    //delete if needed
    
    checkIfPicked();
    //if removed sonar field tell the world this
    
    if(!getAlive()){
        getWorld()->removeSonar();
    }
}

//destructor
SonarKit::~SonarKit(){}

//WATER POOL FUNCTIONS
//Wconstructor
WaterPool::WaterPool(int x, int y, StudentWorld* studentworld): Objects( x, y, studentworld, "temporary", TID_WATER_POOL, right, 1, 2, "WaterPool"){
    setTicksWait(getWorld()->getTicks());
}

void WaterPool::checkIfPicked(){
//if within a radius of 3 of TunnelMan
    if(isLessRad3FromTunnel()){
        //play sound got goodie
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        //increase Water
        getWorld() ->getTunnelMan()->addWater();
        //increase score
        getWorld()->increaseScore(100);
        //set alive to false
        setAlive(false);
        setVisible(false);
    }
}

void WaterPool::deleteIfTempTimeOver(){
    int T = 300-10*getWorld()->getLevel();
    int maxticks;
    if(T >= 100)
        maxticks = T;
    else
        maxticks = 100;
    //if enough ticks have passed delete thw water
    if(getWorld()->getTicks()- getTicksWait() == maxticks){
        setAlive(false);
        setVisible(false);
    }
}

void WaterPool::doSomething(){
    //if dead return
    if(getAlive() == false)
        return;
   
    deleteIfTempTimeOver();
    
    checkIfPicked();
}

WaterPool::~WaterPool(){}

//PROTESTER FUNCTIONS
//constructor
Protesters::Protesters(StudentWorld* studentworld, int imageId, std::string type, int hitPoints): people(imageId, 60, 60, left, 1, 0, hitPoints, studentworld, type){
    setSquaresMoveInCurrentDirection();
    leaveOilField = false;
    //set tick when it last yelled since can only yell every 15 ticks
    ticksLastYell = 0;
    setTicksWait(); //set the number of waiting ticks
    timeWait =  ticksToWaitBetweenMoves + 1;
    lastPerpTurn = 0; //set the last perpendicular turn to 0
}

//setters and getters
void Protesters::setLeaveOil(bool leave){
    leaveOilField = leave;
}

void Protesters::setTimeWait(int time){
    timeWait = time;
}

//gets the leave oil ability
bool Protesters::getLeaveOil() const{
    return leaveOilField;
}

//decrease the squares can move in the current direction
void Protesters::decreaseSquaresMoveInCurrentDirection(){
    numSquaresToMoveInCurrentDirection--;
}

//set squares can move in current direction to 0
void Protesters::decreaseSquaresMoveInCurrentDirectionto0(){
    numSquaresToMoveInCurrentDirection = 0;
}

//solves and then sets of the number of resting ticks there is
void Protesters::setTicksWait(){
int max = 3 - (getWorld()->getLevel()/4);
        if(max > 0)
        ticksToWaitBetweenMoves=max;
        else
            ticksToWaitBetweenMoves = 0;
}

//tells you if you should move, if move then reset the waiting ticks again
bool Protesters::shouldMove(){
    timeWait--;
    if(timeWait == 0){
        timeWait = ticksToWaitBetweenMoves  + 1;
        return true;
    }
    else
        return false;
}

//randomly choose the number of squares to move in the current direction
void Protesters::setSquaresMoveInCurrentDirection(){
    numSquaresToMoveInCurrentDirection = 8 + rand() % 53;
}

//sets when the last yell well
void Protesters::setTicksLastYell(){
    ticksLastYell= getWorld()->getTicks();
}

//this is the function to decrease a protesters hit points and it plays the correct sound if annoyed or if it gives up
bool Protesters::decreaseHitPointsSoundIfAnnoyed(int decrease){
    //dont do anything if in annoyed state
    if(leaveOilField == true)
        return false;
    
    //decrease the hit points by the mount
    decreaseHitPoint(decrease);
    
    if(getHitPoints() > 0){
        GameController::getInstance().playSound(SOUND_PROTESTER_ANNOYED);
    }
    
    if(getHitPoints() <= 0){
        GameController::getInstance().playSound(SOUND_PROTESTER_GIVE_UP);
        leaveOilField = true;
        //here set time wait to 1 means move net tick
        timeWait = 1;
        //return false so can leave
        return true;
    }
    return true;
}


//tells you if it is time where you can  yell
bool Protesters::canYell(){
    //if haven't yelled at all can yell
    if(ticksLastYell == 0)
        return true;
    if((getWorld()->getTicks() - ticksLastYell)/(ticksToWaitBetweenMoves + 1) > 15)
        return true;
    else
        return false;
}

//tells you if you are at a pos that can yell at tunnelman
bool Protesters::posYell() const{
    //need to make sure there is no overlap with tunnel
    //here need to return without moving
    //essentially can yell if there is any overlap with eachother at all
    if(overLapWithTunnel(getX()-1, getY()) && getDirection() == left)
        return true;
    if(overLapWithTunnel(getX()+1, getY()) && getDirection() == right)
        return true;
    if(overLapWithTunnel(getX(), getY() - 1)  && getDirection() == down)
        return true;
    if(overLapWithTunnel(getX(), getY() + 1)  && getDirection() == up)
        return true;
    return false;
}

bool Protesters::yellAtTunnelMan(){
    //if not enough ticks to yell return and return false
    if(!canYell())
        return false;
    
    //if TunnelMan is directly in front of you and you are close enough to yell then yell
    if(posYell()){
        //if in correct direction to yell and enough time has passed and right pos, then yell
        GameController::getInstance().playSound(SOUND_PROTESTER_YELL);
        getWorld()->getTunnelMan()->decreaseHitPointsSoundIfAnnoyed(2);
        ticksLastYell = getWorld()->getTicks();
        return true;
    }
   setDirection(getDirectionOfTunnel());
    return true;
}

//returns if you are a straight distance from tunnelman
bool Protesters::isStraightTunnel() const{
        if(getY() == getWorld()->TunnelManYpos())
            return true;
        if(getX() == getWorld()->TunnelManXpos())
            return true;
    return false;
    //if have same x or y coordinates return true because means it is a straight line to the tunnel man
}

//tells yuou if you have any overlap with tunnelman
bool Protesters::overLapWithTunnel(int x, int y) const{
    //if anywhere overlaps with TunnelMan then found tunnel
    for(int i = getWorld()->TunnelManXpos(); i <= getWorld()->TunnelManXpos() + 3; i++)
        for(int j = getWorld()->TunnelManYpos(); j  <= getWorld()->TunnelManYpos() + 3; j++)
            for(int x1 = x; x1 <= x + 3; x1++)
                for(int y1 = y; y1 <= y + 3; y1++){
                    if(i == x1 && j == y1){
                        //then have an overlap
                        return true;
                    }
                }
    return false;
}

//tells you if there is an empty path to the tunnleman.
bool Protesters::isEmptyPathTunnel() const{
    //now check if there is an empty path to the TunnelMan
    //al checking in for loops because have to check for all 4 coordinates
    if(getDirectionOfTunnel() == right){
        //then check if there are any boulders or Earth in the way
        for(int i = 1; getX() + i < getWorld()->TunnelManXpos(); i+=4){
            if(getWorld()->isEarthHere(getX() + i, getY()))
                return false;
        }
        //also need to check if there is Boulders, plus 1 because don't need to check the current position
        for(int i = 1;getX() + i < getWorld()->TunnelManXpos(); i++){
            if(!getWorld()->isNoBadOverLapBoulder(getX() + i, getY()))
               return false;
               }
    }
    //now need to check the left direction, this means that TunnelMan is to the left
    if(getDirectionOfTunnel() == left){
        for(int i = 1;getWorld()->TunnelManXpos() + i < getX(); i+=4){
            if(getWorld()->isEarthHere(getWorld()->TunnelManXpos() + i, getY()))
                return false;
        }
        //also need to check if there is Boulders
        for(int i = 1; getWorld()->TunnelManXpos() + i < getX(); i++){
            if(!getWorld()->isNoBadOverLapBoulder(getWorld()->TunnelManXpos() + i, getY()))
               return false;
               }
    }
    
    //next check tunnelMan is below
    if(getDirectionOfTunnel() == down){
        for(int i = 1;getWorld()->TunnelManYpos() + i < getY(); i+=4){
            if(getWorld()->isEarthHere(getX(), getWorld()->TunnelManYpos() + i))
                return false;
        }
        //also need to check if there is Boulders
        for(int i = 1;getWorld()->TunnelManYpos() + i < getY(); i++){
            if(!getWorld()->isNoBadOverLapBoulder(getX(), getWorld()->TunnelManYpos() + i))
               return false;
               }
    }
    //finally check if TunnelMan is above you
    if(getDirectionOfTunnel() == up){
        for(int i = 1; getY() + i < getWorld()->TunnelManYpos(); i+=4){
            if(getWorld()->isEarthHere(getX(), getY() + i))
                return false;
        }
        //also need to check if there is Boulders
        for(int i = 1; getY() + i <  getWorld()->TunnelManYpos(); i++){
            if(!getWorld()->isNoBadOverLapBoulder(getX(), getY() + i))
               return false;
               }
    }
    //if get here no obstructions so return true
    return true;
}
//tells you if a direction is empty
bool Protesters::isDirEmpty(Direction dir) const{
    if(isEarthInDir(getX(), getY(), dir ) || !isBoulderInDirBadOverLap(getX(), getY(), dir))
        return false;
    else
        return true;
}

//gives the direction TunnelMan is in used to see if there is a straight path to tunnelman
Barrel::Direction Protesters::getDirectionOfTunnel() const{
    if(getY() == getWorld()->TunnelManYpos() && getX() < getWorld()->TunnelManXpos())
        return right;
    if(getY() == getWorld()->TunnelManYpos() && getX() > getWorld()->TunnelManXpos())
       return left;
if(getX() == getWorld()->TunnelManXpos() && getY() > getWorld()->TunnelManYpos())
       return down;
   else
        return up;
}


//checks if there is a straight empty path to tunnelman, if so moves 1 towards it
bool Protesters::moveIfEmptyStraightPathTunnel(){
    if(!isStraightTunnel())
        return false;
    if(!isEmptyPathTunnel())
        return false;
    //if here straight empty path to tunnelman
    //if making a perpendicualr turn reset when the last perpendicular turn was
    if((getDirection()== left || getDirection() == right) && (getDirectionOfTunnel() == up || getDirectionOfTunnel() == down))
        setLastPerpTurn();
    if((getDirection()== up || getDirection() == down) && (getDirectionOfTunnel() == left || getDirectionOfTunnel() == right))
        setLastPerpTurn();
    
    //change the direciton to be towards tunnelMan
    setDirection(getDirectionOfTunnel());
    //then move one towards TunnelMan
    moveInDir(getX(), getY(), getDirection());
    //then set numSquares to move back to 0
    decreaseSquaresMoveInCurrentDirectionto0();
    return true;
}

//sets the perpendicular turn value
void Protesters::setLastPerpTurn(){
    lastPerpTurn = getWorld()->getTicks();
}

//tells you if you should make a perpendicular turn
bool Protesters::shouldMakePerpTurn() const{
    //this gives number of non resting ticks
    if(((getWorld()->getTicks() - lastPerpTurn) / (ticksToWaitBetweenMoves + 1)) >= 200 && isPerpInter())
        return true;
    else
        return false;
}
//tells you if you are at a perpendicular intersetion
bool Protesters::isPerpInter() const{
    //if left or right see if empty up or down
    if(getDirection() == right || getDirection() == left){
        if(isDirEmpty(up) || isDirEmpty(down))
            return true;
    }
    //if up or down see if empty right or left
    if(getDirection() == up || getDirection() == down){
        if(isDirEmpty(left) || isDirEmpty(right))
            return true;
    }
    //if get here not at an intersection
    return false;
}

//chooses a random perpendicular value
void Protesters::choosePerpDir(){
    int randDir = rand() % 2 + 1;
    
    //if right/left randomly choose a perpendicular direciton that works
    if(getDirection() == right || getDirection() == left){
        if(randDir == 1 && isDirEmpty(up)){
            setDirection(up);
            setLastPerpTurn();
        }
        else if(randDir == 1 && !isDirEmpty(up) && isDirEmpty(down)){
            setDirection(down);
            setLastPerpTurn();
        }
        else if(randDir == 2 && isDirEmpty(down)){
            setDirection(down);
            setLastPerpTurn();
        }
        else if(randDir == 2 && !isDirEmpty(down) && isDirEmpty(up)){
            setDirection(up);
            setLastPerpTurn();
        }
    }
    
    //if up or down randomy choose direciton that works
    //if right/left randomly choose a perpendicular direciton that works
    else if(getDirection() == up || getDirection() == down){
        if(randDir == 1 && isDirEmpty(right)){
            setDirection(right);
            setLastPerpTurn();
        }
        else if(randDir == 1 && !isDirEmpty(right) && isDirEmpty(left)){
            setDirection(left);
            setLastPerpTurn();
        }
        else if(randDir == 2 && isDirEmpty(left)){
            setDirection(left);
            setLastPerpTurn();
        }
        else if(randDir == 2 && !isDirEmpty(left) && isDirEmpty(right)){
            setDirection(right);
            setLastPerpTurn();
        }
    }
}


//this checks if you are at a perpendicular intersection and should make a perpendicular turn, if so it makes a perpendicular move
bool Protesters::takePerpTurnIfCan(){
    //if at a perpendicuarl interaction and time to make a perpendicualr turn
   if(isPerpInter() && shouldMakePerpTurn()){
        //then make a perpendicuarl turn
        choosePerpDir();
        return true;
}
   else
        return false;
}


//picks a random open direction if protester should move
Barrel::Direction Protesters::pickRandomDir(){
    Direction dirBefore = getDirection();
    Direction newDir;
    for(;;){
        //keep picking random direction until find one that works
        int randomNum = 1 + rand() % 4;
        
        //check the direction given, if 1 try right
        if(randomNum == 1 && !isEarthInDir(getX(), getY(), right) && isBoulderInDirBadOverLap(getX(), getY(), right)){
            newDir= right;
            break;
        }
        else if(randomNum == 2 && !isEarthInDir(getX(), getY(), left) && isBoulderInDirBadOverLap(getX(), getY(), left)){
            newDir=left;
            break;
        }
        else if(randomNum == 3 && !isEarthInDir(getX(), getY(), down) && isBoulderInDirBadOverLap(getX(), getY(), down)){
            newDir= down;
            break;
        }
        else if(randomNum == 4 && !isEarthInDir(getX(), getY(), up) && isBoulderInDirBadOverLap(getX(), getY(), up)){
            newDir= up;
            break;
        }
    }
    
    if((dirBefore == left|| dirBefore == right) && (newDir == up ||newDir == down))
        //if made perpendicular turn set this
        setLastPerpTurn();
    if((dirBefore == up|| dirBefore == down) && (newDir== left ||newDir == right))
        //if made perpendicular turn set this
        setLastPerpTurn();
    
    return newDir;
}

//solves for the amount of time a protester should wait if stunned
void Protesters::SetStun(){
    int nMax = (100- (getWorld()->getLevel()* 10));
    if(nMax > 50)
        timeWait = nMax + 1;
    else
        //have to do +1 so thisis the one it moves one
        timeWait = 50 + 1;
}

//this list is all the functions needed to make the queue
//constructor for a coordinate that holds next
coord::coord(int x, int y, coord* parent){
    m_x = x;
    m_y = y;
    m_parent = parent;
}
    //concstructor for type that only holds the coordinates
coordOnly::coordOnly(int x, int y){
    m_x = x;
    m_y = y;
}

//can just return path don;t need to save it since different each tick
std::vector<coordOnly> Protesters::makePath(coord* cord){
    //creates a vector of the path
    std::vector<coordOnly> path;
    //iterator through the chord
    coord* it = cord;
    while(it!= nullptr){
        //loops through getting the path, this is for coordinates only and doesn't hold the pointers
        path.push_back(coordOnly(it->m_x, it->m_y));
        it=it->m_parent; //move to the parent
    }
    //return the path
return path;
}

void Protesters::addValToMazeQueue(std::queue<coord*>& checkmaze, std::string maze[][64], std::vector<coord*>& toDelete) const{
    coord* check = checkmaze.front();
    
    coord* add;
   
    //check if can add left to the queue
    if(!isEarthInDir(check->m_x, check->m_y, left) && isBoulderInDirBadOverLap(check->m_x, check->m_y, left) && maze[check->m_x-1] [check->m_y] != "#"){
        //if not checked dropp a crumb
        maze[check->m_x-1][check->m_y] = "#";
        //then add to that position
        add = new coord(check->m_x - 1, check->m_y, check);
        toDelete.push_back(add);
        //also add it to the queue
        checkmaze.push(add);
    }

    //check if can add right to the queue
    if(!isEarthInDir(check->m_x, check->m_y, right) && isBoulderInDirBadOverLap(check->m_x, check->m_y, right) && maze[check->m_x+1] [check->m_y] != "#"){
        maze[check->m_x+1][check->m_y] = "#";
        add = new coord(check->m_x + 1, check->m_y, check);
        toDelete.push_back(add);
        checkmaze.push(add);
    }
    //check if can add up to the queue
    if(!isEarthInDir(check->m_x, check->m_y, up) && isBoulderInDirBadOverLap(check->m_x, check->m_y, up) && maze[check->m_x][check->m_y + 1] != "#"){
        maze[check->m_x][check->m_y + 1] = "#";
        add = new coord(check->m_x, check->m_y + 1, check);
        toDelete.push_back(add);
        checkmaze.push(add);
    }
    //check if can add down to the queue
    if(!isEarthInDir(check->m_x, check->m_y, down) && isBoulderInDirBadOverLap(check->m_x, check->m_y, down) && maze[check->m_x] [check->m_y - 1] != "#"){
        maze[check->m_x][check->m_y - 1] = "#";
        add = new coord(check->m_x, check->m_y - 1, check);
        toDelete.push_back(add);
        checkmaze.push(add);
    }
    //pop off the last value
    checkmaze.pop();
}

void Protesters::changeQueueDir(std::vector<coordOnly> path){
    //get position to move to
     if((path.end() - 2)->m_x< getX() && (path.end() - 2)->m_y == getY())
         setDirection( left);
     if((path.end() - 2)->m_x > getX() && (path.end() - 2)->m_y == getY())
         setDirection( right );
     if((path.end() - 2)->m_y > getY() && (path.end() - 2)->m_x == getX())
         setDirection(up);
     if((path.end() - 2)->m_y < getY() && (path.end() - 2)->m_x == getX())
         setDirection(down);
     //move to the place the maze found to go to, have to do minus 2 becuase last position is current position
     moveTo((path.end() - 2)->m_x, (path.end() - 2)->m_y);
     
}

//Protester moving to exit
std::vector<coordOnly> Protesters::goToExit(){
    std::string maze[64][64]; //maze to drop a crumb in
    
    //holds everything to delete so just everything that was made in the queue needs to be put into here as well for deletion at the end!
    std::vector<coord*> toDelete;
    
    //need maze checking queue
    std::queue <coord*> checkmaze;
    
    //add the new value to the queue
    coord* add =new coord(getX(),getY(), nullptr);
    toDelete.push_back(add);
    checkmaze.push(add);
    
    //drop a crumb where currently at
    maze[getX()][getY()] = "#";
    
    //tells you if a path has been found
    bool findpath = false;
    
    for(;;){
        //if queue empty didn't find path and leave this is also for debugging purposes
        if(checkmaze.empty()){
            findpath = false;
            break;
        }
        
        //pop off the last value that looked at
        coord* check = checkmaze.front();
        
        //if at exit break and done
        if(check->m_x == 60 && check->m_y == 60){
            findpath = true;
            break;
        }
        
        //then call the function that adds the next value to the queue
        addValToMazeQueue(checkmaze, maze, toDelete);
    }
        
        //this is for if didn't find a path but this is an issue
        if(findpath == false){
            std::vector<coordOnly> empt;
            return empt;
        }
        //get here now have the path;
        //this coordinate is checkmaze.front because if it is the correct spot we didn't pop it of //this is also front because it is the last value there
        std::vector<coordOnly> toReturn = makePath(checkmaze.front());
        
        //delete all of these values
        for(int i = 0; i < toDelete.size(); i++){
            delete toDelete[i];
        }
        return toReturn;
}

//move closer to leave oil field
bool Protesters::leaveOil(){
    if(leaveOilField == false)
        return false;
    else if(pathOut.size() == 0){
        std::vector<coordOnly> path = goToExit();
        //if no way out (mostly because boulder in the way just wait
        pathOut = path;
        if(path.size() == 0)
            return true;
        //change the direction in the one needed
        changeQueueDir(path);
        
        //delete the last thing in the queue because tat was already done
        pathOut.pop_back();
        
        //changes value so return true
        return true;
   }
   else{
        //change the direction in the one needed
        changeQueueDir(pathOut);
        
        //delete the last thing in the queue because tat was already done
        pathOut.pop_back();
        return true;
    }
}

bool Protesters::doRegAndHardProtestFirstHalf(){
    //if not alive return
    if(!getAlive())
        return true;
    //if not turn to move return
    if(!shouldMove())
        return true;
    
    //return true if leaving oil field but also if at the right position at the end then have it turn off
    if(leaveOil()){
        if(getX() == 60 && getY() == 60){
            setAlive(false);
            setVisible(false);
            //decrease the number of protesters 
            getWorld()->decreaseNumberProtesters();
        }
            return true;
        }
    //if at yelling position yell if can (if can't yell just return) this I am still asking questions about
    if(posYell()){
        //if yelled at TunnelMan return
        yellAtTunnelMan();
        //return
        return true;
           
    }
    return false;
}

void Protesters::doRegAndHardProtestSecondHalf(){
    //move on towards tunnelman if there is an empty straight path towards him
    if(moveIfEmptyStraightPathTunnel())
        return;

    //if get here it can't directly see tunnelman, shouldn't be leaving the oil field, is not dead, and should be moving so decrement the movement
    decreaseSquaresMoveInCurrentDirection();
    
    //if no more squares to move set it to a different direction pick a new direction an number of values to move in that direction
    if(numSquaresToMoveInCurrentDirection <= 0){
        setDirection(pickRandomDir());
        setSquaresMoveInCurrentDirection();
    }
    else{
        //check if need to make a perpendicualr turn
        takePerpTurnIfCan();
        setSquaresMoveInCurrentDirection();
    }

    //here if can move in direction move
    if(isDirEmpty(getDirection()))
        moveInDir(getX(), getY(), getDirection());
    
    //else if can't move in that direction set the squares to move in that direction to 0
    else{
        decreaseSquaresMoveInCurrentDirectionto0();
    }
}


//destructor 
Protesters::~Protesters(){}

//REGULAR PROTESETER FUNCTIONS
//regularProtester function constructor
regularProtester::regularProtester(StudentWorld* studentworld): Protesters(studentworld, TID_PROTESTER, "regularProtester", 5){
}

void regularProtester::squirt(){
    //if not in the leave oil field state
    if(!getLeaveOil()){
        //if still alive after kill set it to a stun state and increase the score
        SetStun();
        //only increase score it it causes them to give up
       
    }
    //if in leave oil field then increase score
    if(getLeaveOil()){
        //then increase score
        getWorld()->increaseScore(100);
    }
}

void regularProtester::getGold(){
    if(!getLeaveOil()){
        //say get gold
        GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
        //increase the score
        getWorld()->increaseScore(25);
        //bribe to off oil state
        setLeaveOil(true);
        //reset tick times here one means will run next tick
        setTimeWait(1);
    }
}
void regularProtester::doSomething(){
    //do first half of common
    //if any of it returns true it did it so return
    if(doRegAndHardProtestFirstHalf())
        return;
    doRegAndHardProtestSecondHalf();
}

//return the path of a coordinate
std::vector<coordOnly> hardCoreProtester::moveToTunnel(){
    std::string maze[64][64]; //maze to drop a crumb in
    //everything in here is uninitialized
    
    //holds everything to delete so just everything that was made in the queue needs to be put into here as well for deletion at the end
    std::vector<coord*> toDelete;
    
    //need maze checking queue
    std::queue <coord*> checkmaze;
    
    coord* add = new coord(getX(),getY(), nullptr);
    toDelete.push_back(add);
    checkmaze.push(add);
    
    //drop a crumb where currently at
    maze[getX()][getY()] = "#";
    
//this tells you if you can find a path
    bool findpath = false;
    
    for(;;){
        //if queue empty didn't find path and leave this is also for debugging purposes
        if(checkmaze.empty()){
            findpath = false;
            break;
        }
        
        //get the first thing in the maze to check again
        coord* check = checkmaze.front();
  
        if(overLapWithTunnel(check->m_x, check->m_y)){
            findpath = true;
            break;
        }
        
        //then call the function that adds the next value to the queue
        addValToMazeQueue(checkmaze, maze, toDelete);
    }
        
        //this is for if didn't find a path then don't go throuh
    if(findpath == false){
         std::vector<coordOnly> empt;
        return empt;
    }
        //then create the vector of the path from the coordinates given
        std::vector<coordOnly> toReturn = makePath(checkmaze.front());
        
        //delete all of these values
        for(int i = 0; i < toDelete.size(); i++){
            delete toDelete[i];
        }
        return toReturn;
    }


//destructor
regularProtester::~regularProtester(){}

//HARD CORE PROTESTER FUNCTIONS
hardCoreProtester::hardCoreProtester(StudentWorld* studentworld):Protesters(studentworld, TID_HARD_CORE_PROTESTER, "hardCoreProtester", 20){}

void hardCoreProtester::squirt(){
    if(!getLeaveOil())
        //if still alive after kill set it to a stun state
        SetStun();
    if(getLeaveOil()){
        getWorld()->increaseScore(250);
    }
}

void hardCoreProtester::getGold(){
    //say get gold
    GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
    //increase the score
    getWorld()->increaseScore(50);
    //bribe to off oil state
    //then reset the ticks to just make him pause
    int stare = 100-getWorld()->getLevel()*10;
    //reset waiting time
    if(stare > 50)
        //make sure to always do the plus 1 so that it is the correct amount
        setTimeWait(stare + 1);
    else
        setTimeWait(50 + 1);
}

bool hardCoreProtester::moveInTunnelDirIfCan(){
    int M = 16 + getWorld()->getLevel()* 2;
    //if to close leave
    if(getWorld()->getEuclidDist(getX(),getY(), getWorld()->TunnelManXpos(), getWorld()->TunnelManYpos()) <= 4)
        return false;
    
    //if the euclidiean distance is greater than M+4 away then it can NOT get to the TunnelMan in less than M steps because it is just to far away.
if(getWorld()->getEuclidDist(getX(),getY(), getWorld()->TunnelManXpos(), getWorld()->TunnelManYpos()) > M + 4)
    return false;
    
    std::vector<coordOnly> path = moveToTunnel();
    
 
    
    //if path size <= 2 there is nowwhere to go
    if(path.size () <=2 )
        return true;

   
    if(path.size() - 1 <= M){
        changeQueueDir(path);

        //changes value so return true
        return true;
    }
    return false;
}

//do Something
void hardCoreProtester::doSomething(){
    //do first half of common
    //if any of it returns true it did it so return
    if(doRegAndHardProtestFirstHalf())
        return;
    
    //if in correct distance tunnel move towards nim
   if(moveInTunnelDirIfCan())
       return;
    
    //implement middle stuff that is different
    doRegAndHardProtestSecondHalf();
}
//destructor
hardCoreProtester::~hardCoreProtester(){}
