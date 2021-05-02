#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>

class Actor;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class Earth;
class TunnelMan;

class StudentWorld : public GameWorld
{
public:
	// Name: StudentWorld
	// Input: string
	// Output: none
	// Description: constructor
	StudentWorld(std::string assetDir);

	// Name: ~StudentWorld
	// Input: none
	// Output: none
	// Description: destructor
	virtual ~StudentWorld();

	// Name: init
	// Input: none
	// Output: int representing status
	// Description: initialize
	virtual int init();

	// Name: move
	// Input: none 
	// Output: int representing status
	// Description: ask all the actors to do something
	virtual int move();

	// Name: cleanUp
	// Input: none
	// Output: none
	// Description: empties the field
	virtual void cleanUp();

	// Name: displayText
	// Input: none
	// Output: none
	// Description: displays text on top of the screen
	void displayText();

	// Name: updateGrid
	// Input: none
	// Output: none
	// Description: mark the position of earth and boulder
	void updateGrid();

	// Name: getEarthPtr
	// Input: coordinates
	// Output: earth ptr
	// Description: returns a pointer to the earth at specific position
	Earth* getEarthPtr(int x, int y);

	// Name: removeEarth
	// Input: coordinates
	// Output: none
	// Description: removes earth at specific position
	void removeEarth(int x, int y);

	// Name: withinEarth
	// Input: coordinates
	// Output: bool
	// Description: determines if an object is completely within earth
	bool withinEarth(int x, int y);

	// Name: getGrid
	// Input: coordinates
	// Output: int representing id of object
	// Description: return the type of object at the grid
	int getGrid(int x, int y);

	// Name: getOil
	// Input: none
	// Output: int
	// Description: returns the amount of oil left
	int getOil() const;

	// Name: createSquirt
	// Input: none
	// Output: none
	// Description:create a squirt object when the player presses space
	void createSquirt();

	// Name: createGold
	// Input: none
	// Output: none
	// Description:create a gold object when the player presses tab
	void createGold();

	// Name: reveal
	// Input: none
	// Output: none
	// Description: reveals hidden objects within distance of 12
	void reveal();

	// Name: tunnelManX
	// Input: none
	// Output: int 
	// Description: x coordinate of tunnelman
	int tunnelManX();

	// Name: tunnelManY
	// Input: none
	// Output: int 
	// Description: y coordinate of tunnelman
	int tunnelManY();


	// Name: ThreeFromProtester
	// Input: object ptr
	// Output: pointer of protester within distance of three
	// Description: find protester within distance of three
	Actor* ThreeFromProtester(Actor* object) const;

	// Name: annoyProtester
	// Input: object ptr and points to decrement
	// Output: none
	// Description: annoy the protester
	void annoyProtester(Actor* object, int points);

	// Name: annoyTunnelMan
	// Input: points to decrement
	// Output: none
	// Description: annoy the tunnelman
	void annoyTunnelMan(int points);

	// Name: ThreeFromBoulder
	// Input: coordinates
	// Output: bool
	// Description: whether the point is within 3 from the boulder
	bool ThreeFromBoulder(int x, int y);

	// Name: pickUp
	// Input: object ptr
	// Output: none
	// Description: pick up a goodie
	void pickUp(Actor* object);

	// Name: too close
	// Input: coordinates
	// Output: bool
	// Description: distributed object too close to another distributed object
	bool tooClose(int x, int y);

	// Name: invalidLocationForPool
	// Input: coordinate
	// Output: bool
	// Description: whether the pool is not completely on empty ground
	bool invalidLocationForPool(int x, int y);
private:
	int grid[VIEW_WIDTH][VIEW_HEIGHT];
	std::vector<Actor*> m_ActorPtr;
	Earth* m_EarthPtr[VIEW_WIDTH][VIEW_HEIGHT - 4]; // stores all the earth objects
	TunnelMan* m_TunnelManPtr; // stores the tunnelman onject
	int m_water;
	int m_numGold;
	int m_numOil;
	int m_sonar;
	int m_numBoulder;
	int m_ticksSinceLastProtester;
	int m_ticksBetweenProtesters;
	int m_numProtester;
	int m_maxProtester;
	int m_probabilityHardcore;
};

#endif // STUDENTWORLD_H_
