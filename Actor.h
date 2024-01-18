#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <queue>
#include <vector>
#include <string>

//declaration to help it not be cyclical 
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//actor class that everything is dervied from
class actor: public GraphObject{
public:
    //constructor
    actor(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth, StudentWorld* studentworld, std::string types);
    
    //getters and setters
    //getter to get world in
    StudentWorld* getWorld() const;
    //tells you the alive status
    bool getAlive() const;
    //use to set alive status
    void setAlive(bool set);
    //gets the tyoe of actor
    std::string getType() const;
    
    //these are world checkers so that objects can see what is going on around them in the oil field
    //checks if there is earth in the way you want to move
    bool isBoulderInDirBadOverLap(int x, int y, Direction dir) const;
    //tells you if you are within a radius of three of tunnelman
    bool isEarthInDir(int x, int y, Direction dir) const;
    //tells  you if a boulder is less than rad 3 in a specific direction
    bool isLessRad3FromTunnel() const;
    
    
    //these are for actions that are commonly used in many different types
    //if can move in direction move one square in a direction
    void moveInDir(int x, int y, Direction dir);
    //sets you to visible if within a radius of 4
    bool setVisIf4();
    
    //these are virtual functions, most of these are for interactions between different objects, because they are between different objects they are often being called from the objects array in StudentWorld which is of actors, so these have to be here and not in the other functions for interactionr reasons
    //this is for objects gold, boulder and squirt which interact with protesters but also will need to be called by object
   virtual void interactWithProtester(actor* protest);
    //different objects have different things to do when hit points decreased.
    virtual bool decreaseHitPointsSoundIfAnnoyed(int decrease);
    //so that the different protesters can be squirted differently
    virtual void squirt();
    //so that the different protesters can get gold. These were put here because they are accessed by objects which is pointers to actors, returns
    virtual void getGold();
    //tells you if you have been picked up by the tunnelMan
    virtual void checkIfPicked();
    
    //do something makes virtual
    void virtual doSomething();
    
    //destructor
    virtual ~actor();

private:
   //member variables
    //pointer to world
    StudentWorld* m_StudentWorld;
    //tells you if an object is alive
    bool m_isAlive;
    //tells you the type of the object
    std::string m_type;

};

//Earth class to hold Earth
class Earth: public actor{
public:
    //constructor
    Earth(int startX, int startY, StudentWorld* studentworld);
    
    //destructor
    virtual ~Earth();
};


//these are for things with hit points
class people: public actor{
public:
    people(int imageID, int startX, int startY, Direction startDirection,  double size, unsigned int depth, int hitpoints, StudentWorld* studentworld, std::string type);
    
    //setters and getters
    //return hitpoints 
    int getHitPoints() const;
    //allows you to set HitPoint
    void decreaseHitPoint(int decrease);
    //allows you to set HitPoint to 0
    void setHitPoint0();
    
    //destructor
    virtual ~people();
private:
    //holds number of hit points
    int m_hitpoints;
};

class TunnelMan: public people{
public:
//constructor
    TunnelMan(StudentWorld* studentworld);
    
    //setters and getters
    //gets water squirts
    int getWaterSquirt() const;
    //get sonary charges
    int getSonarCharges() const;
    //get gold nuggets
    int getGoldNuggets() const;
    //if comes close to sonary charga
    void addSonarCharge();
    //add number of gold nuggets
    void addGoldNuggets();
    //add water to TunnelMan
    void addWater();
    
    //this is because TunnelMans must be hit, it is virtual because it occurs in many classes
    virtual bool decreaseHitPointsSoundIfAnnoyed(int decrease);
    
    virtual void doSomething();

    //destructor
    virtual ~TunnelMan();
private:
    //member variables
    int m_watersquirts;
    int m_sonarcharges;
    int m_goldnuggets;
    
    //call in do something to destroy Earth
    void digEarth();
    //tells function what to do if key is pressed
    void ifKey(int key);
    //drop GoldNugget if have one for Protesters to use
    void dropGoldNugget();
    //for do Something with a Sonar Charge
    void doSonarCharge();
    //decreases hit points when you press escape
    void decreaseHitPointsescp();
};

//class Objects for all objects (except the Barrel to derive from)
class Objects: public actor{
public:
    //constructor
    Objects(int x, int y, StudentWorld* studentworld, std::string State, int TID, Direction direction, double size, unsigned int depth, std::string type);
    
    //getters and setters
    //string getstate
    std::string getState() const;
    //setsate
    void setState(std:: string set);
    //get tickets waiting
    int getTicksWait() const;
    //set tickts waiting 
    void setTicksWait(int time);
    
    //used to delete objects if their time as being temp, virtual because many have different temp times 
    virtual void deleteIfTempTimeOver();
    
    //destructor
    virtual ~Objects();
private:
    //keeps track of its state
    std::string m_state;
    
    //keeps track of ticks at a specific moment
    int m_ticksStartWaiting;
};

class Boulders: public Objects{
public:
    //constructor
    Boulders(int x, int y, StudentWorld* studentworld);
    
    //virtual functions
    //this is for objects gold, boulder and squirt which interact with protesters but also will need to be called by object
   virtual void interactWithProtester(actor* protest);
    virtual void doSomething();
    
    //destructor
    virtual ~Boulders();
    
private:
    //checks if there is a Boulder underneath you, this is just so a boulder stops falling if it falls onto another boulder
    bool checkBoulders(int x, int y) const;
};


class Barrel: public actor{
public:
    //constructor
    Barrel(int x, int y, StudentWorld* studentworld);
    
    //tells you if within a radius of three so tunnelman can pick up
    virtual void checkIfPicked();
    
    void doSomething();
    
    //destructor
   virtual  ~Barrel();
};

class GoldNugget: public Objects{
public:
    //need to set a Visible because if dropped by TunnelMan it is visible and if just created it is not visible
    GoldNugget(int x, int y, StudentWorld* studentworld, bool Visible, std::string states, std::string CanPickUp);
        
    //virtual functions for interaction
    virtual void interactWithProtester(actor* protest);
    //tells you if within a radius of three so tunnelman can pick up
    virtual void checkIfPicked();
    //delete if finished the temp time
    virtual void deleteIfTempTimeOver();
    
    virtual void doSomething();

    //destructor
    virtual ~GoldNugget();
    
private:
    //holeds who can pick it up
    std::string m_whoCanPickUp;
};

class Squirt: public Objects{
public:
    //constructors
    Squirt(int x, int y, StudentWorld* studentworld, Direction startDirection);
    
    //virtual functions
    virtual void interactWithProtester(actor* protest);
    
    //doSomething
    virtual void doSomething();
    
    //destructor
    virtual ~Squirt();
private:
    int squaresToTravel;
};

class SonarKit: public Objects{
public:
    //constructor
    SonarKit( StudentWorld* studentworld);
    
    //tells you if within a radius of three so tunnelman can pick up
    virtual void checkIfPicked();
    //delete if finished the temp time
    virtual void deleteIfTempTimeOver();
    
    //do something
    virtual void doSomething();
    
    //destructor
    virtual ~SonarKit();
};

class WaterPool: public Objects{
public:
    WaterPool(int x, int y, StudentWorld* studentworld);
    
    //virtual functions
    //tells you if within a radius of three so tunnelman can pick up
    virtual void checkIfPicked();
    //delete if finished the temp time
    virtual void deleteIfTempTimeOver();
    
    virtual void doSomething();
    
    //destructor
   virtual  ~WaterPool();
};

//this is used for the queue based search to hold a coordinate, this coordinate also holds a pointer to the next value
struct coord{
public:
    coord(int x, int y, coord* parent);
    int m_x;
    int m_y;
    //holds a pointer to the next value
    coord* m_parent;
};

//a struct of coordinates without pointers to the next values to make things easier
struct coordOnly{
    coordOnly(int x, int y);
    int m_x;
    int m_y;
};

class Protesters: public people{
public:
    //constructor
    Protesters(StudentWorld* studentworld, int imageID, std::string type, int hitPoints);
    
    //setters and getters
    //sets the time to wait between things
    void setTimeWait(int time);
    //sets the leave oil status
    void setLeaveOil(bool leave);
    //gets the leaves oil stats
    bool getLeaveOil() const;
    //set squares can move in current direction to 0
    void decreaseSquaresMoveInCurrentDirectionto0();
    
    //functions for the Protesters Do Somethings
    //calls all the stuff regular and hardcore protesters have in common for first part doSomething
    bool doRegAndHardProtestFirstHalf();
    //calls all the stuff regular and Harcore protester hvae in common in second half break it up because in middle different
    void doRegAndHardProtestSecondHalf();
    

    //tells you if you are going to overlap with the tunnelman
    bool overLapWithTunnel(int x, int y) const;
   //stuns the protesters for when squirted
    void SetStun();
    
    //queue based maze functions
    //this adds a value to mazes queues
    void addValToMazeQueue(std::queue<coord*>& checkmaze, std::string maze[][64], std::vector<coord*>& toDelete) const;
    //creates the path vector from a coordinate
    std::vector<coordOnly> makePath(coord* cord);
    //this moves you to the correct direction from the maze queue
    void changeQueueDir(std::vector<coordOnly> path);

    
    //allows  you to decrease hit points and tell if something is annoyed
    virtual bool decreaseHitPointsSoundIfAnnoyed(int decrease);
    
    //destructor
    virtual ~Protesters();
    
private:
//member variables
    //set to tell you if you are in the leave OilField state or not
    bool leaveOilField;
    //gives the number of squares left to move in a direction
    int numSquaresToMoveInCurrentDirection;
    //holds how much time normally set to wait between moves
    int ticksToWaitBetweenMoves;
    //holds the time of the last yell
    int ticksLastYell;
    //holds the time of the last perpendicular turn
    int lastPerpTurn;
    //sets actual time must wait between moves 
    int timeWait;
    //holds the path out
    std::vector<coordOnly> pathOut;
    
    //functions to figure out when to move
    //tells you if this is the tick you should move in
    bool shouldMove();
    //set the ticks to wait
    void setTicksWait();
    
    //functios to yell
    //sets the tick it yelled
    void setTicksLastYell();
    //tells you if you are at a pos that can yell at tunnelman
    bool posYell() const;
    //tell you if 4 units from tunnelMan
    bool isGreater4UnitsTunnel() const;
    //tells you if there is enough ticks left to yell
    bool canYell();
    //yells At TunnelMan if can
    bool yellAtTunnelMan();
    
    
    //functions to see if you have a straight path towards tunnelman to more towards
    //tells you if ou have a straight path to the tunnleman
    bool isStraightTunnel() const;
    //tells you if you have an empty straight path to the tunnelman
    bool isEmptyPathTunnel() const;
    //tells you if a direction is empty
    bool isDirEmpty(Direction dir) const;
    //gives the direction of tunnelman
    Direction getDirectionOfTunnel() const;
    //moves you if you have an empty straight path to the tunnelman
    bool moveIfEmptyStraightPathTunnel();
    
    
    //functions to tells you if you are at a perpendicuarl intersection and if you should move
    //tells you if perpenciduarl
    bool isPerpInter() const;
    //resets when the last perpendicuarl turn was
    void setLastPerpTurn();
    //tells you if it is time to make a perpendicular turn
    bool shouldMakePerpTurn() const;
    //gives a perpendicular direction randomly
    void  choosePerpDir();
    //takes a perpencidualr turn if can
    bool takePerpTurnIfCan();
    
    //functions for movement
    //sets the number of squares you can move in the current direction
    void setSquaresMoveInCurrentDirection();
    //decrease the squares can move in the current direction
    void decreaseSquaresMoveInCurrentDirection();
    //picks a random direction to move in
    Direction pickRandomDir();

//functions for queue based oil field leaving
    std::vector<coordOnly> goToExit();
    bool leaveOil();
};

class regularProtester: public Protesters
{
public:
    //constructor
    regularProtester(StudentWorld* studentworld);
    
    //so that the different protesters can be squirted differently
    virtual void squirt();
    //so that the different protesters can get gold
    virtual void getGold();

    virtual void doSomething();
    
    //destructor
    virtual ~regularProtester();
};

class hardCoreProtester: public Protesters{
public:
    //constructor
    hardCoreProtester(StudentWorld* studentworld);
    
    //virtuals to interacting with other values
    //so that the different protesters can be squirted differently
    virtual void squirt();
    //so that the different protesters can get gold
    virtual void getGold();
    
    virtual void doSomething();
    
    //destructor
    virtual ~hardCoreProtester();
private:
    //function that creates the vector of coordinates if moving towards TunnelMan
    std::vector<coordOnly> moveToTunnel();
//function that tells how to move towards tunnelman if can
    bool moveInTunnelDirIfCan();
};



#endif // ACTOR_H_
