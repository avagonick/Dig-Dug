#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include "Actor.h"



// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
        ticks = 0;
	}

    //functions given to use
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //setters and getters
    //getsEarth, this returns the value in the Array of earth
    Earth* getearths(int i, int j) const;
    //returns object vector
    std::vector<actor*> getObjects() const;
    //gets ticks that have passed since the beginning
    int getTicks() const;
    //gives number of Barrels in oil field, so TunnelMan can know if he has picked up all the barrels or not
    int numBarrels() const;
    //decrease number of barrels by 1, so TunnelMan can decrease them when he picks them up
    void changeBarrels();
    //get tunnel man so can use functions on him
    TunnelMan* getTunnelMan();
    //gives TunnelMans X position
    int TunnelManXpos() const;
    //gives TunnelMans y position
    int TunnelManYpos() const;
    //addGoldNugget so that TunnelMan can add a gold nugget to the field
    
    //changing things in the field
    void addGoldNugget();
    //adds a squirt object, this is for TunnelMan to use
    void addSquirt(int x, int y);
    //removes a sonar from the field
    void removeSonar();
    //decreases number of protesters
    void decreaseNumberProtesters();

    
    
    //functions ot tell you information about the oil field
    //gives the Euclidean distance between two objects, public because it is a function used alot
    double getEuclidDist(double x1, double y1, double x2, double y2) const;
    //tells you if Earth is at a position
    bool isEarthHere(int x, int y) const;
    //tells you if valid coordinates
    bool isValidCoords(int x, int y) const;
    //does the right things if within rad 3 of a protester 
    void isRad3Protest(actor * act) const;
    //tells you if you have icorrect overlap with boulder from what was said on piazza
    bool isNoBadOverLapBoulder(int x, int y) const;

    //destructor
    ~StudentWorld();
private:
    //member vairables
    //vector to hold all other objects
    std::vector<actor*> objects;
    //a pointer to a tunnelman
    TunnelMan* tunnel;
    //2d array to hold Earth pointers since Earth is only 1 wide should hold in here 
    Earth* earths[64][64];
    //holds ticks that have passed
    int ticks;
    //keeps count of the number of Barrels left to hold
    int numBarrelsLeft;
    //keeps track if there is Sonar in the field
    bool sonarInField;
    //keeps track of ticks since last added protester
    int ticksSinceLastProtester;
    //keeps track of number of protesters
    int numberProtesters;
    //keeps track of max num of Protesters;
    int MaxNumProtest;
    //keep track ticks that have to pass
    int ticksToAddNewProtest;
    
    //innitiliazer functions
    //gives the max number of protesters
    int getTargetProtesters();
    //used to solve for info needed to add the original protesters
    void initProtesters();
    //add protesters
    void addOrigProtester();
    //initializes all the earth
    void addEarth();
    //inits all the boulders
    void setBoulders();
    //used in set Boulders
    //deletes a 4x4 block of Earth
    void delete4x4(int x, int y);
    //tells you if an object is within a radius 6 of another
    bool isNoObjectOverlap(int x, int y) const;
    //tells you if a spot is open for an object to be placed
    bool isGoodSpot(int x, int y) const;
    //sets Barrels
    void setBarrels();
    //adds Nugggets to oil field
    void addNuggets();
    
    //these are all used during move
    //functions for getting number of protesters to add each mobe
    //tells you how many ticks must be between before adding a new protester
    int tickSoCanAddNewProtest() const;
    //adds a newProtesterIfShould
    void addNewProtesterIfShould();
    //tells you if you should add a hardcore Protester
    bool shouldAddHardCore() const;
    
    //functions for adding goodies
 
    //creates water pool in add goodie
    void createWaterPool();
    //adds gooides
    void addGoodie();
    
    //to Display the text at the top
    std::string setDisplayText();
    
//destructor functions
    //deletes all the earth
    void deleteEarth();
};


#endif // STUDENTWORLD_H_
