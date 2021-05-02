#include "GraphObject.h"
#include "StudentWorld.h"
#include <string>
#include <iomanip>
#include <sstream>
#include <cstdlib>

#include "Actor.h"

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++)
			grid[i][j] = EMPTY;
}

StudentWorld::~StudentWorld()
{
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT - 4; j++)
			delete m_EarthPtr[i][j]; // delete all earth objects
	while (m_ActorPtr.size() != 0)
	{
		// delete all actors
		delete m_ActorPtr[m_ActorPtr.size() - 1];
		m_ActorPtr.pop_back();
	}

	delete m_TunnelManPtr; // delete the tunnelman object
}

int StudentWorld::init()
{
	//initialize number of objects
	if (5 - getLevel() / 2 >= 2)
		m_numGold = 5 - getLevel() / 2;
	else
		m_numGold = 2;

	if (2 + getLevel() <= 21)
		m_numOil = 2 + getLevel();
	else
		m_numOil = 21;

	if (2 + getLevel() / 2 <= 9)
		m_numBoulder = 2 + getLevel() / 2;
	else
		m_numBoulder = 9;

	if (200 - getLevel() >= 25)
		m_ticksBetweenProtesters = 200 - getLevel();
	else
		m_ticksBetweenProtesters = 25;

	if (2 + getLevel() * 1.5 <= 15)
		m_maxProtester = 2 + getLevel() * 1.5;
	else
		m_maxProtester = 15;

	if (30 + getLevel() * 10 <= 90)
		m_probabilityHardcore = 30 + getLevel() * 10;
	else
		m_probabilityHardcore = 90;

	m_ticksSinceLastProtester = m_ticksBetweenProtesters;

	// tunnelman:
	m_TunnelManPtr = new TunnelMan(this); // create a tunnelman object

	// earth:
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT - 4; j++)
			if (j < 4 || i < VIEW_WIDTH / 2 - 2 || i >= VIEW_WIDTH / 2 + 2)
				// create earth on each grid except top 4 rows and a tunnel in the middle
			{
				m_EarthPtr[i][j] = new Earth(this, i, j);
				grid[i][j] = EARTH;
			}
			else
				m_EarthPtr[i][j] = nullptr;

	// boulder:
	int x = 999;
	int y = 999;
	for (int k = 0; k < m_numBoulder; k++)
	{
		do
		{
			x = rand() % 64;
			y = rand() % 60;

		} while (!withinEarth(x, y) || tooClose(x, y));
		Actor* boulderPtr = new Boulder(this, x, y);
		for (int i = x; i < x + 4; i++)
			for (int j = y; j < y + 4; j++)
				removeEarth(i, j);

		m_ActorPtr.push_back(boulderPtr);
	}

	// Oil:
	for (int k = 0; k < m_numOil; k++)
	{
		do
		{
			x = rand() % 64;
			y = rand() % 60;

		} while (!withinEarth(x, y) || tooClose(x, y));
		Actor* oilPtr = new Oil(this, x, y);
		m_ActorPtr.push_back(oilPtr);
	}

	// Gold:
	for (int k = 0; k < m_numGold; k++)
	{
		do
		{
			x = rand() % 64;
			y = rand() % 60;

		} while (!withinEarth(x, y) || tooClose(x, y));
		Actor* goldPtr = new Gold(this, x, y);
		m_ActorPtr.push_back(goldPtr);
	}

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// check if the player is still alive
	if (m_TunnelManPtr->isAlive() == false)
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (m_numOil == 0)
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
	// reset the grid
	updateGrid();
	// display the text on the top
	displayText();
	// make the tunnelman do something
	m_TunnelManPtr->doSomething();
	// make all actors do something
	for (int i = 0; i < m_ActorPtr.size(); i++)
		m_ActorPtr[i]->doSomething();
	// remove dead objects:
	for (int i = 0; i < m_ActorPtr.size(); i++)
	{
		if (m_ActorPtr[i]->isAlive() == false)
		{
			delete m_ActorPtr[i];
			for (int j = i; j < m_ActorPtr.size() - 1; j++)
				m_ActorPtr[j] = m_ActorPtr[j + 1];
			m_ActorPtr.pop_back();
		}
	}
	// add new actors:
	// protester:
	if (m_numProtester < m_maxProtester)
	{
		if (m_ticksSinceLastProtester >= m_ticksBetweenProtesters)
		{
			Actor* protesterPtr;
			if (rand() % 100 < m_probabilityHardcore)
				protesterPtr = new HardcoreProtester(this);
			else
				protesterPtr = new RegularProtester(this);
			m_ActorPtr.push_back(protesterPtr);
			m_numProtester++;
			m_ticksSinceLastProtester = 0;
		}
		else
		{
			m_ticksSinceLastProtester++;
		}
	}
	// goodies:
	int G = getLevel() * 25 + 300;
	if (rand() % G == 0)
	{
		Actor* goodie;
		if (rand() % 5 == 0)
			goodie = new Sonar(this);
		else
		{
			int x = 999;
			int y = 999;
			do
			{
				x = rand() % 64;
				y = rand() % 60;

			} while (invalidLocationForPool(x, y));
			goodie = new Pool(this, x, y);
		}
		m_ActorPtr.push_back(goodie);
	}


	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT - 4; j++)
			delete m_EarthPtr[i][j]; // delete all earth objects
	while (m_ActorPtr.size() != 0)
	{
		// delete all actors
		delete m_ActorPtr[m_ActorPtr.size() - 1];
		m_ActorPtr.pop_back();
	}
	delete m_TunnelManPtr; // delete the tunnelman object
}

void StudentWorld::displayText()
{
	ostringstream oss;
	oss << "Lvl: " << setw(2) << getLevel() <<
		" Lives: " << getLives() <<
		" Hlth: " << setw(3) << (m_TunnelManPtr->getHitPoints() * 10) <<
		"% Wtr: " << setw(2) << m_TunnelManPtr->getWater() <<
		" Gld: " << setw(2) << m_TunnelManPtr->getGold() <<
		" Oil Left: " << setw(2) << getOil() <<
		" Sonar: " << setw(2) << m_TunnelManPtr->getSonar() <<
		" Scr: " << setw(6) << setfill('0') << getScore();
	string text = oss.str();
	setGameStatText(text);
}

void StudentWorld::updateGrid()
{
	// empty the grid except for earth
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++)
			if (grid[i][j] != EARTH)
				grid[i][j] = EMPTY;
	// update the grid so that all boulders are marked in the grid
	for (int i = 0; i < m_ActorPtr.size(); i++)
		if (m_ActorPtr[i]->ID() == BOULDER)
		{
			for (int x = m_ActorPtr[i]->getX(); x < m_ActorPtr[i]->getX() + 4; x++)
				for (int y = m_ActorPtr[i]->getY(); y < m_ActorPtr[i]->getY() + 4; y++)
					grid[x][y] = BOULDER;
		}

}

Earth* StudentWorld::getEarthPtr(int x, int y)
{
	return m_EarthPtr[x][y]; // return a pointer to earth on grid (x, y)
}

void StudentWorld::removeEarth(int x, int y)
{
	// if there is earth in the target grid, set the earth as invisible
	if (m_EarthPtr[x][y] != nullptr)
	{
		delete m_EarthPtr[x][y];
		m_EarthPtr[x][y] = nullptr;
		grid[x][y] = EMPTY;
		playSound(SOUND_DIG);
	}

}

bool StudentWorld::withinEarth(int x, int y)
{
	for (int i = x; i < x + 4; i++)
		for (int j = y; j < y + 4; j++)
			if (grid[i][j] != EARTH)
				return false;
	return true;
}

int StudentWorld::getGrid(int x, int y)
{
	return grid[x][y];
}

int StudentWorld::getOil() const
{
	return m_numOil;
}

void StudentWorld::createSquirt()
{
	Actor* squirtPtr = new Squirt(this, m_TunnelManPtr->getX(),
		m_TunnelManPtr->getY(), m_TunnelManPtr->getDirection());
	playSound(SOUND_PLAYER_SQUIRT);
	m_ActorPtr.push_back(squirtPtr);
}

void StudentWorld::createGold()
{
	Actor* goldPtr = new Gold(this, m_TunnelManPtr->getX(),
		m_TunnelManPtr->getY(), true, false, false);
	m_ActorPtr.push_back(goldPtr);
}

void StudentWorld::reveal()
{
	for (int i = 0; i < m_ActorPtr.size(); i++)
	{
		if (m_TunnelManPtr->distanceWith(m_ActorPtr[i]->getX(), m_ActorPtr[i]->getY()) <= 12)
			m_ActorPtr[i]->setVisible(true);
	}
}

int StudentWorld::tunnelManX()
{
	return m_TunnelManPtr->getX();
}

int StudentWorld::tunnelManY()
{
	return m_TunnelManPtr->getY();
}


Actor* StudentWorld::ThreeFromProtester(Actor* object) const
{
	for (int i = 0; i < m_ActorPtr.size(); i++)
	{
		if (m_ActorPtr[i]->ID() == PROTESTER &&
			object->distanceWith(m_ActorPtr[i]->getX(), m_ActorPtr[i]->getY()) <= 3)
			return m_ActorPtr[i];
	}
	return nullptr;
}

void StudentWorld::annoyProtester(Actor* object, int points)
{
	object->annoyed(points);
}

void StudentWorld::annoyTunnelMan(int points)
{
	m_TunnelManPtr->decHitPoints(points);
	if (m_TunnelManPtr->getHitPoints() <= 0)
		m_TunnelManPtr->setDead();
}

bool StudentWorld::ThreeFromBoulder(int x, int y)
{
	for (int i = 0; i < m_ActorPtr.size(); i++)
	{
		if (m_ActorPtr[i]->ID() == BOULDER &&
			m_ActorPtr[i]->distanceWith(x, y) <= 3)
			return true;
	}
	return false;
}

void StudentWorld::pickUp(Actor* object)
{
	object->setDead();
	if (object->ID() == OIL)
	{
		increaseScore(1000);
		playSound(SOUND_FOUND_OIL);
		m_numOil--;
	}
	else if (object->ID() == SONAR)
	{
		increaseScore(75);
		m_TunnelManPtr->pickUpSonar();
		playSound(SOUND_GOT_GOODIE);
	}
	else if (object->ID() == WATERPOOL)
	{
		increaseScore(100);
		m_TunnelManPtr->pickUpWater();
		playSound(SOUND_GOT_GOODIE);
	}
	else if (object->ID() == GOLD)
	{
		increaseScore(10);
		m_TunnelManPtr->pickUpGold();
		playSound(SOUND_GOT_GOODIE);
	}
}

bool StudentWorld::tooClose(int x, int y)
{
	for (int i = 0; i < m_ActorPtr.size(); i++)
		if (m_ActorPtr[i]->ID() == OIL || m_ActorPtr[i]->ID() == GOLD || m_ActorPtr[i]->ID() == BOULDER)
			if (m_ActorPtr[i]->distanceWith(x, y) < 6)
				return true;
	return false;
}

bool StudentWorld::invalidLocationForPool(int x, int y)
{
	for (int i = x; i < x + 4; i++)
		for (int j = y; j < y + 4; j++)
			if (grid[i][j] == EARTH || grid[i][j] == BOULDER)
				return true;
	return false;
}


