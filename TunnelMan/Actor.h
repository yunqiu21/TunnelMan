#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <stack>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/************************************************************************************
boulder state constants
*************************************************************************************/
const int STABLE = 0;
const int WAITING = 1;
const int FALLING = 2;

/************************************************************************************
actor type constants
*************************************************************************************/
const int EMPTY = 0;
const int EARTH = 1;
const int BOULDER = 2;
const int TUNNELMAN = 3;
const int PROTESTER = 4;
const int SQUIRT = 5;
const int WATERPOOL = 6;
const int SONAR = 7;
const int OIL = 8;
const int GOLD = 9;

/************************************************************************************
protester state constants
*************************************************************************************/
const int REST = 0;
const int NORMAL = 1;
const int LEAVING = 2;

class StudentWorld;

// Base class for all actors
class Actor : public GraphObject
{
public:
	// Name: Actor
	// Input: pointer to student world, actor id, image id, starting position, 
	// starting direction, size, depth, hit points
	// Output: none
	// Description: constructor
	Actor(StudentWorld* sw, int id, int imageID, int startX, int startY,
		Direction dir = right, double size = 1.0, unsigned int depth = 0,
		int hitPoints = 0);
	// Name: ~Actor
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~Actor();
	// Name: doSomething
	// Input: none
	// Output: none
	// Description: allows the objects to take actions
	virtual void doSomething();
	// Name: getWorld
	// Input: none
	// Output: pointer to studentWorld
	// Description: return the StudentWorld that stores objects
	StudentWorld* getWorld() const;
	// Name: isAlive
	// Input: none
	// Output: a bool telling whether the actor is alive
	// Description: checks if the actor is alive
	bool isAlive() const;
	// Name: setDead
	// Input: none
	// Output: none
	// Description: set the actor as dead so that it can be cleaned up
	void setDead();
	// Name: blockedBy
	// Input: actor type id
	// Output: bool telling whether the actor is blocked
	// Description: tells whether the actor is blocked by an actor of certain type
	bool blockedBy(int id) const;
	// Name: collideWithBoulder
	// Input: none
	// Output: boolean
	// Description: tells whether the actor is at the same position as a boulder
	bool collideWithBoulder() const;
	// Name: distanceWith
	// Input: the coordinates of another object
	// Output: a double representing distance
	// Description: get the distance between the actor (not earth) and a given point
	double distanceWith(int x, int y) const;
	// whether the actor is x away from tunnelman
	bool xFromTunnelMan(int x) const;
	// Name: facingTunnelMan
	// Input: object
	// Output: bool
	// Description: check if the object is directly facing the tunnelman
	bool facingTunnelMan() const;
	// direction of tunnelman if he is in straight sight of object
	GraphObject::Direction straightSight();
	// Name: getHitPoints
	// Input: none
	// Output: an int representing hit points
	// Description: return the hit points remaining
	int getHitPoints() const;
	// Name: decHitPoints
	// Input: an int representing the amount to decrement
	// Output: none
	// Description: decrease hit points by a certain amount
	void decHitPoints(int points);
	// Name: ID
	// Input: none
	// Output: an int representing the actor type (id constants are defined at the top)
	// Description: return the id of the actor 
	int ID() const;
	// Name: moveOneStep
	// Input: none
	// Output: none
	// Description: move one step in the current direction
	void moveOneStep();
	// Name: bribed
	// Input: none
	// Output: none
	// Description: used by protesters who picked up a gold nugget
	virtual void bribed();
	// Name: annoyed
	// Input: an int representing the hit points to decrement
	// Output: none
	// Description: used by protesters annoyed by squirt or boulder
	virtual void annoyed(int amount);
private:
	StudentWorld* m_sw; // the StudentWorld that stores game objects
	bool m_alive;
	int m_hitPoints;
	int m_id;
};

class Earth :public Actor
{
public:
	// Name: Earth
	// Input: studentWorldPointer, coordinates
	// Output: none
	// Description: constructor
	Earth(StudentWorld* sw, int x, int y);
	// Name: ~Earth
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~Earth();
};

class TunnelMan :public Actor
{
public:
	// Name: TunnelMan
	// Input: studentWorld pointer
	// Output: none
	// Description: constructor
	TunnelMan(StudentWorld* sw);
	// Name: ~TunnelMan
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~TunnelMan();
	// Name: doSomething
	// Input: none
	// Output: none
	// Description: let the tunnelman take actions
	virtual void doSomething();
	// Name: getWater
	// Input: none
	// Output: int representing amount of water
	// Description: returns the amount of water
	int getWater() const;
	// Name: getGold
	// Input: none
	// Output: int representing number of gold nuggets
	// Description: return number of gold nuggets
	int getGold() const;
	// Name: getSonar
	// Input: none
	// Output: int representing number of sonar
	// Description: returns number of sonar
	int getSonar() const;
	// Name: pickUpSonar
	// Input: none
	// Output: none
	// Description: increase score and the number of sonar
	void pickUpSonar();
	// Name: pickUpGold
	// Input: none
	// Output: none
	// Description: increase score and the number of gold
	void pickUpGold();
	// Name: pickUpWater
	// Input: none
	// Output: none
	// Description: increase score and the amount of water
	void pickUpWater();
private:

	int m_water;
	int m_gold;
	int m_sonar;
};

class Boulder :public Actor
{
public:
	// Name: Boulder
	// Input: studentWorldPointer, coordinates
	// Output: none
	// Description: constructor
	Boulder(StudentWorld* sw, int x, int y);
	// Name: ~Boulder
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~Boulder();
	// Name: doSomething
	// Input: none
	// Output: none
	// Description: let the boulder take action
	virtual void doSomething();

private:

	int m_state;
	int m_ticksWaiting;
};


class Squirt :public Actor
{
public:
	// Name: Squirt
	// Input: studentWorld pointer, coordinates, direction
	// Output: none
	// Description: constructor
	Squirt(StudentWorld* sw, int x, int y, Direction dir);
	// Name: ~Squirt
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~Squirt();
	// Name: doSomething
	// Input: none
	// Output: none
	// Description: let the squirt take action
	virtual void doSomething();
private:
	int m_travelDistance;
	int m_distanceTraveled;
};

class Goodies :public Actor
{
public:
	// Name: Goodies
	// Input: type, image id, studentWorld pointer, coordinates, whether it is temporary,
	// hidden or can be picked up by tunnelman
	// Output: none
	// Description: constructor
	Goodies(int id, int imageID, StudentWorld* sw, int x, int y, bool temporary,
		bool hidden, bool pickByTunnelMan);
	// Name: ~Goodies
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~Goodies();
	// Name: doSomething
	// Input: none
	// Output: none
	// Description: let the goodie take action
	virtual void doSomething();
private:
	bool m_temporary;
	bool m_hidden;
	bool m_pickByTunnelMan;
	int m_lifeLeft;
};

class Oil :public Goodies
{
public:
	// Name: Oil
	// Input: studentWorldPointer, coordinates
	// Output: none
	// Description: constructor
	Oil(StudentWorld* sw, int x, int y);
	// Name: ~Oil
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~Oil();
};


class Gold :public Goodies
{
public:
	// Name: Gold
	// Input: sw pointer, coordinates, whether it is temporary, hidden or 
	// can be picked up by a tunnelman (or protester if false)
	// Output: none
	// Description: constructor
	Gold(StudentWorld* sw, int x, int y, bool temporary = false,
		bool hidden = true, bool pickByTunnelMan = true);
	// Name: ~Gold()
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~Gold();
};

class Sonar :public Goodies
{
public:
	// Name: Sonar
	// Input: sw pointer
	// Output: none
	// Description: constructor
	Sonar(StudentWorld* sw);
	// Name:~Sonar
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~Sonar();
};

class Pool :public Goodies
{
public:
	// Name: Pool
	// Input: studentWorldPointer, coordinates
	// Output: none
	// Description: constructor
	Pool(StudentWorld* sw, int x, int y);
	// Name: ~Pool
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~Pool();
};

/*******************************************************************************
Protester
*******************************************************************************/


class Protester :public Actor
{
public:
	// Name: Protester
	// Input: type of actor, sw ptr, image id, hit points, whether it is hardcore
	// Output: none
	// Description: constructor
	Protester(int id, StudentWorld* sw, int imageID, int hitPoints, bool hardcore = false);
	// Name: ~Protester
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~Protester();
	// Name: doSomething
	// Input: none
	// Output: none
	// Description: let the boulder take action
	virtual void doSomething();
	// Name: findPathTo
	// Input: end point, two ints representing the next square to go to
	// Output: int representing the number of steps
	// Description: find the shortest way to the destination
	int findPathTo(int ex, int ey, int& nextX, int& nextY);
	// Name: leave
	// Input: none
	// Output: none
	// Description: make one move towards the exit
	void leave();
	// Name: canTurnCCW
	// Input: none
	// Output: bool representing whether it can turn counterclockwise and move
	// Description: check if it can turn counterclockwise and move at least one step
	bool canTurnCCW();
	// Name: canTurnCW
	// Input: none
	// Output: bool representing whether it can turn clockwise and move
	// Description: check if it can turn clockwise and move at least one step
	bool canTurnCW();
	// Name: canMove
	// Input: none
	// Output: true means there's no obstacles blocking the way
	// Description: checks whether it can move forward without obstacles
	bool canMove();
	// Name: turnCCW
	// Input: none
	// Output: none
	// Description: change its direction counterclockwise
	void turnCCW();
	// Name: turnCW
	// Input: none
	// Output: none
	// Description: change its direction clockwise
	void turnCW();
	// Name: doSomethingSpecial
	// Input: none
	// Output: bool telling if hardcore protester has taken action
	// Description: for Hardcore Protester to check if the tunnelman is within a certain number of steps
	virtual bool doSomethingSpecial();
	// Name: bribed
	// Input: none
	// Output: none
	// Description: get bribed when see a gold nugget
	virtual void bribed();
	// Name: annoyed
	// Input: amount to decrement hit points
	// Output: none
	// Description: gets annoyed
	virtual void annoyed(int amount);
	// Name: setRestTicks
	// Input: number of ticks to rest
	// Output: none
	// Description: reset the number of ticks to remain in resting state
	void setRestTicks(int ticks);
private:
	bool m_hardcore;
	int m_state;
	int m_squaresToMove;
	int m_ticksNotTurned;
	int m_ticksNotShout;
	int m_ticksToRest;
	int m_maxTicksToRest;

	struct Node
	{
		Node(int X, int Y);
		int x;
		int y;
		bool discovered;
		Node* prev;
	};
	Node* node[VIEW_WIDTH][VIEW_HEIGHT];


};

class RegularProtester :public Protester
{
public:
	// Name: RegularProtester
	// Input: studentWorldPointer
	// Output: none
	// Description: constructor
	RegularProtester(StudentWorld* sw);
	// Name: ~RegularProtester
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~RegularProtester();
	// Name: bribed
	// Input: none
	// Output: none
	// Description: bribed by gold nugget
	virtual void bribed();
};

class HardcoreProtester :public Protester
{
public:
	// Name: HardcoreProtester
	// Input: studentWorldPointer
	// Output: none
	// Description: constructor
	HardcoreProtester(StudentWorld* sw);
	// Name: ~HardcoreProtester
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~HardcoreProtester();
	// Name: doSomethingSpecial
	// Input: none
	// Output: bool telling whether it has taken action
	// Description: follow the tunnelman if withing M legal steps
	virtual bool doSomethingSpecial();
	// Name: bribed
	// Input: none
	// Output: none
	// Description: bribed by gold nugget
	virtual void bribed();
};


#endif // ACTOR_H_

