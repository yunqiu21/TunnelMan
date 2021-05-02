#include "Actor.h"
#include "StudentWorld.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <stack>

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp



/************************************************************************
Actor
*************************************************************************/

// Constructor
Actor::Actor(StudentWorld* sw, int id, int imageID, int startX, int startY,
	Direction dir, double size, unsigned int depth, int hitPoints)
	:GraphObject(imageID, startX, startY, dir, size, depth)
{
	m_id = id;
	m_sw = sw;
	m_alive = true;
	setVisible(true);
	m_hitPoints = hitPoints;
}

// Destructor
Actor::~Actor() { }

void Actor::doSomething() { }

StudentWorld* Actor::getWorld() const
{
	return m_sw;
}

bool Actor::isAlive() const
{
	return m_alive;
}

void Actor::setDead()
{
	m_alive = false;
}

bool Actor::blockedBy(int id) const
{
	// for each of the four directions
	if (getDirection() == left)
	{
		if (getX() == 0) // if it is at the edge and cannot move further
			return true;
		for (int y = getY(); y < getY() + 4; y++)
			// if any of the grids in front of it is occupied by a certain actor
			if (getWorld()->getGrid(getX() - 1, y) == id)
				// then this actor is blocked by that actor
				return true;
	}
	if (getDirection() == right)
	{
		if (getX() == 60)
			return true;
		for (int y = getY(); y < getY() + 4; y++)
			if (getWorld()->getGrid(getX() + 4, y) == id)
				return true;
	}
	if (getDirection() == down)
	{
		if (getY() == 0)
			return true;
		for (int x = getX(); x < getX() + 4; x++)
			if (getWorld()->getGrid(x, getY() - 1) == id)
				return true;
	}
	if (getDirection() == up)
	{
		if (getY() == 60)
			return true;
		for (int x = getX(); x < getX() + 4; x++)
			if (getWorld()->getGrid(x, getY() + 4) == id)
				return true;
	}
	return false; // no obstacles found
}

bool Actor::collideWithBoulder() const
{
	// whether there is a boulder at the same position as this actor
	return(getWorld()->getGrid(getX(), getY()) == BOULDER);
}


double Actor::distanceWith(int x, int y) const
{
	// distance between two objects with size 1
	return sqrt((x - getX()) * (x - getX()) + (y - getY()) * (y - getY()));
}

bool Actor::xFromTunnelMan(int x) const {
	return (distanceWith(getWorld()->tunnelManX(), getWorld()->tunnelManY()) <= x);
}

bool Actor::facingTunnelMan() const {
	// for all four directions
	if (getDirection() == left)
	{
		// within 4 squares vertically and horizontally and facing
		// the direction of the tunnelman
		if (getX() > getWorld()->tunnelManX() &&
			getX() <= getWorld()->tunnelManX() + 4 &&
			getY() >= getWorld()->tunnelManY() - 4 &&
			getY() <= getWorld()->tunnelManY() + 4)
			return true;
	}
	else if (getDirection() == right)
	{
		if (getX() < getWorld()->tunnelManX() &&
			getX() >= getWorld()->tunnelManX() - 4 &&
			getY() >= getWorld()->tunnelManY() - 4 &&
			getY() <= getWorld()->tunnelManY() + 4)
			return true;
	}
	else if (getDirection() == down)
	{
		if (getY() > getWorld()->tunnelManY() &&
			getY() <= getWorld()->tunnelManY() + 4 &&
			getX() >= getWorld()->tunnelManX() - 4 &&
			getX() <= getWorld()->tunnelManX() + 4)
			return true;
	}
	else if (getDirection() == up)
	{
		if (getY() < getWorld()->tunnelManY() &&
			getY() >= getWorld()->tunnelManY() - 4 &&
			getX() >= getWorld()->tunnelManX() - 4 &&
			getX() <= getWorld()->tunnelManX() + 4)
			return true;
	}

	return false;
}

GraphObject::Direction Actor::straightSight()
{
	// tunnelman is on the left
	if (getWorld()->tunnelManX() < getX() &&
		getWorld()->tunnelManY() == getY())
	{
		for (int i = getWorld()->tunnelManX(); i < getX(); i++)
			for (int j = getY(); j < getY() + 4; j++)
				if (getWorld()->getGrid(i, j) == EARTH)
					return GraphObject::none;
		for (int i = getWorld()->tunnelManX(); i < getX(); i++)
			if (getWorld()->ThreeFromBoulder(i, getY()))
				return GraphObject::none;
		return GraphObject::left;
	}
	// tunnelman is on the right
	else if (getWorld()->tunnelManX() < getX() &&
		getWorld()->tunnelManY() == getY())
	{
		for (int i = getX(); i < getWorld()->tunnelManX(); i++)
			for (int j = getY(); j < getY() + 4; j++)
				if (getWorld()->getGrid(i, j) == EARTH)
					return none;
		for (int i = getX(); i < getWorld()->tunnelManX(); i++)
			if (getWorld()->ThreeFromBoulder(i, getY()))
				return none;
		return right;
	}
	// tunnelman is right below
	else if (getWorld()->tunnelManY() < getY() &&
		getWorld()->tunnelManX() == getX())
	{
		for (int i = getX(); i < getX() + 4; i++)
			for (int j = getWorld()->tunnelManY(); j < getY(); j++)
				if (getWorld()->getGrid(i, j) == EARTH)
					return none;
		for (int j = getWorld()->tunnelManY(); j < getY(); j++)
			if (getWorld()->ThreeFromBoulder(getX(), j))
				return none;
		return down;
	}
	// tunnelman is right above
	else if (getWorld()->tunnelManX() < getX() &&
		getWorld()->tunnelManY() == getY())
	{
		for (int i = getX(); i < getX() + 4; i++)
			for (int j = getY(); j < getWorld()->tunnelManY(); j++)
				if (getWorld()->getGrid(i, j) == EARTH)
					return none;
		for (int j = getY(); j < getWorld()->tunnelManY(); j++)
			if (getWorld()->ThreeFromBoulder(getX(), j))
				return none;
		return up;
	}
	return none;
}

int Actor::getHitPoints() const
{
	return m_hitPoints;
}

void Actor::decHitPoints(int points)
{
	m_hitPoints -= points;
}

int Actor::ID() const
{
	return m_id;
}

void Actor::moveOneStep()
{
	if (getDirection() == left)
	{
		moveTo(getX() - 1, getY()); // move 1 step to the left
	}
	else if (getDirection() == right)
	{
		moveTo(getX() + 1, getY()); // move 1 step to the right
	}
	else if (getDirection() == down)
	{
		moveTo(getX(), getY() - 1); // move 1 step down
	}
	else if (getDirection() == up)
	{
		moveTo(getX(), getY() + 1); // move 1 step up
	}
}

void Actor::bribed()
{
}

void Actor::annoyed(int amount)
{
}



/************************************************************************
Earth
*************************************************************************/

// Constructor
Earth::Earth(StudentWorld* sw, int x, int y)
	:Actor(sw, EARTH, TID_EARTH, x, y, right, 0.25, 3)
{ }

// Destructor
Earth::~Earth() { }

/************************************************************************
TunnelMan
*************************************************************************/

// Constructor
TunnelMan::TunnelMan(StudentWorld* sw)
	:Actor(sw, TUNNELMAN, TID_PLAYER, 30, 60, right, 1.0, 0, 10) // created at (30, 60)
{
	m_water = 5;
	m_gold = 0;
	m_sonar = 1;
}

// Destructor
TunnelMan::~TunnelMan() { }

void TunnelMan::doSomething()
{
	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		// user hit a key this tick!

		switch (ch)
		{
		case KEY_PRESS_LEFT:
			// if the tunnelman is not faced in the direction
			if (getDirection() != left)
			{
				// turn to the indicated direction but not move
				setDirection(left);
				return;
			}
			// if the tunnelman is already facing the direction
			if (getX() > 0 && !getWorld()->ThreeFromBoulder(getX() - 1, getY()))
				// if it is allowed to move in that direction
			{
				moveTo(getX() - 1, getY()); // move 1 step to the left
				// the tunnelman will cover the 4 grids to his left after he moves
				// any remaining earth on those 4 grids needs to be removed
				for (int y = getY(); y < 60 && y < getY() + 4; y++)
					// remove any remaining earth by calling StudentWorld::removeEarth()
					getWorld()->removeEarth(getX(), y);
			}
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
			{
				setDirection(right);
				return;
			}
			if (getX() < 60 && !getWorld()->ThreeFromBoulder(getX() + 1, getY()))
			{
				moveTo(getX() + 1, getY());
				for (int y = getY(); y < 60 && y < getY() + 4; y++)
					getWorld()->removeEarth(getX() + 3, y);
			}
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down)
			{
				setDirection(down);
				return;
			}
			if (getY() > 0 && !getWorld()->ThreeFromBoulder(getX(), getY() - 1))
			{
				moveTo(getX(), getY() - 1);
				for (int x = getX(); x < getX() + 4; x++)
					getWorld()->removeEarth(x, getY());
			}
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up)
			{
				setDirection(up);
				return;
			}
			if (getY() < 60 && !getWorld()->ThreeFromBoulder(getX(), getY() + 1))
			{
				moveTo(getX(), getY() + 1);
				// we will not consider the top 4 rows, where there is no earth
				if (getY() <= 56)
					for (int x = getX(); x < getX() + 4; x++)
						getWorld()->removeEarth(x, getY() + 3);
			}
			break;
		case KEY_PRESS_SPACE: // squirt
			if (m_water > 0)
			{
				// create a squirt object in studentWorld
				getWorld()->createSquirt();
				// decrement the amount of water
				m_water--;
			}
			break;
		case  KEY_PRESS_TAB:
			if (m_gold > 0)
			{
				// drop a gold nugget
				getWorld()->createGold();
				// decrement the amount of gold
				m_gold--;
			}
			break;
		case 'z':
		case 'Z': // use sonar
			if (m_sonar > 0)
			{
				// reveal all hidden objects within distance 12
				getWorld()->reveal();
				// decrement number of sonar
				m_sonar--;
			}
			break;
		case KEY_PRESS_ESCAPE:
			// give up
			setDead();
			break;
		}
	}
}



int TunnelMan::getWater() const
{
	return m_water;
}

int TunnelMan::getGold() const
{
	return m_gold;
}



int TunnelMan::getSonar() const
{
	return m_sonar;
}

void TunnelMan::pickUpSonar()
{	
	m_sonar++;
}

void TunnelMan::pickUpGold()
{
	m_gold++;
}

void TunnelMan::pickUpWater()
{
	m_water += 5;
}

/************************************************************************
Boulder
*************************************************************************/

Boulder::Boulder(StudentWorld* sw, int x, int y)
	:Actor(sw, BOULDER, TID_BOULDER, x, y, down, 1.0, 1)
{
	m_state = STABLE;
	m_ticksWaiting = 0;
}

Boulder::~Boulder()
{

}

void Boulder::doSomething()
{
	if (!isAlive()) // check whether it is alive
		return;
	if (m_state == STABLE)
	{
		for (int x = getX(); x < getX() + 4; x++)
		{
			// if there is at least one earth below it
			if (getWorld()->getEarthPtr(x, getY() - 1) != nullptr)
				return;
		}
		// no earth below
		m_state = WAITING;
	}
	else if (m_state == WAITING)
	{
		m_ticksWaiting++;
		if (m_ticksWaiting == 30)
		{
			// waited 30 ticks
			m_state = FALLING;
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}

	}
	else if (m_state == FALLING)
	{
		if (blockedBy(EARTH) || blockedBy(BOULDER))
		{
			// stop and disappear
			setDead();
			return;
		}
		// move down
		moveTo(getX(), getY() - 1);		
		// annoy tunnelman if within distance of 3
		if (xFromTunnelMan(3))
			getWorld()->annoyTunnelMan(100);
		else
		{
			// annoy all protesters within distance of 3
			while (getWorld()->ThreeFromProtester(this) != nullptr)
			{
				getWorld()->annoyProtester(getWorld()->ThreeFromProtester(this), 100);
			}
		}
	}

}



/************************************************************************
Squirt
*************************************************************************/

Squirt::Squirt(StudentWorld* sw, int x, int y, Direction dir)
	:Actor(sw, SQUIRT, TID_WATER_SPURT, x, y, dir, 1.0, 1)
{
	m_travelDistance = 4;
	m_distanceTraveled = 0;
}

Squirt::~Squirt()
{

}

void Squirt::doSomething()
{
	// traveled full distance or meet obstacle
	if (m_distanceTraveled == 4 || blockedBy(EARTH) || collideWithBoulder())
	{
		setDead();
		return;
	}
	moveOneStep();
	m_distanceTraveled++;
	// annoy protester
	if (getWorld()->ThreeFromProtester(this) != nullptr)
	{
		getWorld()->annoyProtester(getWorld()->ThreeFromProtester(this), 2);
		setDead();
	}
}

/************************************************************************
Goodies
*************************************************************************/

Goodies::Goodies(int id, int imageID, StudentWorld* sw, int x, int y, bool temporary,
	bool hidden, bool pickByTunnelMan)
	:Actor(sw, id, imageID, x, y, right, 1.0, 2)
{
	m_temporary = temporary;
	m_hidden = hidden;
	m_pickByTunnelMan = pickByTunnelMan;
	if (m_temporary)
	{
		// lifetime for temporary objects except for gold, which is always 100
		if (300 - getWorld()->getLevel() * 10 >= 100 && ID() != GOLD)
			m_lifeLeft = 300 - getWorld()->getLevel() * 10;
		else
			m_lifeLeft = 100;
	}
}

Goodies::~Goodies() {}

void Goodies::doSomething()
{

	// if temporary, change its lifetime
	if (m_temporary)
	{
		if (m_lifeLeft <= 0)
			setDead();
		else
			m_lifeLeft--;
	}
	if (!isAlive())
		return;
	// if hidden, check if discovered
	if (m_hidden && xFromTunnelMan(4))
		setVisible(true);
	// if pick-upable by tunnelman, check if picked up
	if (m_pickByTunnelMan)
	{
		if (xFromTunnelMan(3))
			getWorld()->pickUp(this);
	}

	// if pick-upable by protester, check if picked up
	// it has to be gold
	else if (getWorld()->ThreeFromProtester(this) != nullptr)
		// bribe the protester with the gold
		getWorld()->ThreeFromProtester(this)->bribed();

}

/************************************************************************
derived classes of Goodies
*************************************************************************/

Oil::Oil(StudentWorld* sw, int x, int y)
	:Goodies(OIL, TID_BARREL, sw, x, y, false, true, true)
{
	setVisible(false);
}

Oil::~Oil()
{
}

Gold::Gold(StudentWorld* sw, int x, int y, bool temporary,
	bool hidden, bool pickByTunnelMan)
	:Goodies(GOLD, TID_GOLD, sw, x, y, temporary, hidden, pickByTunnelMan)
{
	if (hidden == true)
		setVisible(false);
}

Gold::~Gold()
{
}



Sonar::Sonar(StudentWorld* sw)
	:Goodies(SONAR, TID_SONAR, sw, 0, 60, true, false, true)
{
}

Sonar::~Sonar()
{
}

Pool::Pool(StudentWorld* sw, int x, int y)
	:Goodies(WATERPOOL, TID_WATER_POOL, sw, x, y, true, false, true)
{

}

Pool::~Pool()
{
}

/************************************************************************
Protester
*************************************************************************/

Protester::Protester(int id, StudentWorld* sw, int imageID, int hitPoints, bool hardcore)
	:Actor(sw, id, imageID, 60, 60, left, 1.0, 0, hitPoints)
{
	m_hardcore = hardcore;
	m_state = NORMAL;
	m_squaresToMove = rand() % 53 + 8; // set number of squares to move before turning
	if (3 - (getWorld()->getLevel()) / 4 >= 0)
		m_maxTicksToRest = 3 - (getWorld()->getLevel()) / 4;
	else
		m_maxTicksToRest = 0;

	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++)
			node[i][j] = new Node(i, j); // initialize 2D array
}

Protester::~Protester()
{
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++)
			delete node[i][j];
}

void Protester::doSomething()
{

	if (!isAlive())
		return;

	if (m_state == REST)
	{
		m_ticksToRest--;
		if (m_ticksToRest <= 0)
		{
			// rest state ends
			if (getHitPoints() <= 0)
				m_state = LEAVING;
			else
				m_state = NORMAL;
		}

	}
	else if (m_state == LEAVING)
	{
		m_state = REST;
		m_ticksToRest = m_maxTicksToRest;
		if (getX() == 60 && getY() == 60)
		{
			// arrive at the exit
			setDead();
		}
		else
			// move toward exit
			leave();
	}
	else if (m_state == NORMAL)
	{
		m_state = REST;
		m_ticksToRest = m_maxTicksToRest;
		// facing directly
		if (facingTunnelMan())
		{
			// yell at tunnelman
			if (m_ticksNotShout == 0)
			{
				getWorld()->annoyTunnelMan(2);
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				m_ticksNotShout = 15;
			}
			else
			{
				// stop yelling for 15 ticks
				m_ticksNotShout--;
			}
			return;
		}
		// for hardcore protesters that sense the signal of tunnelman
		if (doSomethingSpecial())
			return;
		// straight line of sight
		if (straightSight() != none && !xFromTunnelMan(4))
		{
			setDirection(straightSight());
			moveOneStep();
			m_squaresToMove = 0;
			return;
		}
		m_squaresToMove--;
		// change direction
		if (m_squaresToMove <= 0)
		{
			do {
				switch (rand() % 4)
				{
				case 0:
					setDirection(left);
					break;
				case 1:
					setDirection(right);
					break;
				case 2:
					setDirection(down);
					break;
				case 3:
					setDirection(up);
					break;
				}
			} while (!canMove());
			m_squaresToMove = rand() % 53 + 8;
		}
		// at an intersection
		else if (canTurnCCW() || canTurnCW())
		{
			if (m_ticksNotTurned <= 0) // time to make a turn
			{
				if (canTurnCCW() && canTurnCW())
				{
					switch (rand() % 2)
					{
					case 0:
						turnCCW();
						break;
					case 1:
						turnCW();
						break;
					}
				}
				// only counterclockwise works
				else if (canTurnCCW())
					turnCCW();
				// only clockwise works
				else
					turnCW();
				m_ticksNotTurned = 200;
			}
			else
			{
				m_ticksNotTurned--;
			}
		}
		if (!canMove())
		{
			m_squaresToMove = 0;
			return;
		}
		moveOneStep();
		m_ticksNotTurned--;

	}
}

Protester::Node::Node(int X, int Y)
{
	x = X;
	y = Y;
	discovered = false;
	prev = nullptr;
}

int Protester::findPathTo(int ex, int ey, int& nextX, int& nextY)
{
	int numSteps = 0;
	// reset the nodes
	for (int i = 0; i < VIEW_WIDTH; i++)
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			node[i][j]->discovered = false;
			node[i][j]->prev = nullptr;
		}

	Node* start = node[getX()][getY()];
	start->discovered = true;

	queue<Node*> path;
	path.push(start);
	while (!path.empty())
	{
		Protester temp(PROTESTER, getWorld(), TID_PROTESTER, 5);
		Node* cur = path.front();
		path.pop();
		if (cur->x == ex && cur->y == ey)
		{
			while (cur->prev != nullptr)
			{
				numSteps++; // push the last step onto the stack
				nextX = cur->x;
				nextY = cur->y;
				cur = cur->prev; // go to the previous step
			}
			return numSteps;
		}
		temp.moveTo(cur->x, cur->y);
		// for all four directions
		temp.setDirection(left);
		// if can move in that direction
		if (temp.canMove() && node[cur->x - 1][cur->y]->discovered == false)
		{
			node[cur->x - 1][cur->y]->discovered = true;
			node[cur->x - 1][cur->y]->prev = cur;
			path.push(node[cur->x - 1][cur->y]); // put that node onto the queue
		}
		temp.setDirection(right);
		if (temp.canMove() && node[cur->x + 1][cur->y]->discovered == false)
		{
			node[cur->x + 1][cur->y]->discovered = true;
			node[cur->x + 1][cur->y]->prev = cur;
			path.push(node[cur->x + 1][cur->y]);
		}
		temp.setDirection(down);
		if (temp.canMove() && node[cur->x][cur->y - 1]->discovered == false)
		{
			node[cur->x][cur->y - 1]->discovered = true;
			node[cur->x][cur->y - 1]->prev = cur;
			path.push(node[cur->x][cur->y - 1]);
		}
		temp.setDirection(up);
		if (temp.canMove() && node[cur->x][cur->y + 1]->discovered == false)
		{
			node[cur->x][cur->y + 1]->discovered = true;
			node[cur->x][cur->y + 1]->prev = cur;
			path.push(node[cur->x][cur->y + 1]);
		}
	}
	return -1; // no path (won't happen)
}

void Protester::leave()
{
	if (getY() == 60) // at the top, move right
		setDirection(right);
	else if (getX() == 30) // in the middle tunnel, move to the top
		setDirection(up);
	else
	{
		// get the direction to go to the exit
		int nextX;
		int nextY;
		findPathTo(60, 60, nextX, nextY);
		if (nextX == getX() - 1)
			setDirection(left);
		if (nextX == getX() + 1)
			setDirection(right);
		if (nextY == getY() - 1)
			setDirection(down);
		if (nextY == getY() + 1)
			setDirection(up);
	}
	moveOneStep();
}

bool Protester::canTurnCCW()
{
	Direction original = getDirection();
	turnCCW();
	bool can = canMove();
	setDirection(original);
	return can;
}

bool Protester::canTurnCW()
{
	Direction original = getDirection();
	turnCW();
	bool can = canMove();
	setDirection(original);
	return can;
}

bool Protester::canMove()
{
	return !(blockedBy(EARTH) || blockedBy(BOULDER));
}

void Protester::turnCCW()
{
	// left to down, down to right, right to up, up to left
	if (getDirection() == left)
		setDirection(down);
	else if (getDirection() == right)
		setDirection(up);
	else if (getDirection() == down)
		setDirection(right);
	else if (getDirection() == up)
		setDirection(left);
}

void Protester::turnCW()
{
	// left to up, up to right, right to down, down to left
	if (getDirection() == left)
		setDirection(up);
	else if (getDirection() == right)
		setDirection(down);
	else if (getDirection() == down)
		setDirection(left);
	else if (getDirection() == up)
		setDirection(right);
}

bool Protester::doSomethingSpecial()
{
	// only for hardcore protesters
	return false;
}

void Protester::bribed()
{
	getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
}

void Protester::annoyed(int amount)
{
	if (m_state == LEAVING) // won't further annoy
		return;
	decHitPoints(amount);
	getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
	int ticks;
	if (100 - getWorld()->getLevel() * 10 >= 50)
		ticks = (100 - getWorld()->getLevel() * 10);
	else
		ticks = 50;
	setRestTicks(ticks); // stop for some time
	if (getHitPoints() <= 0) // enter leaving state
	{
		m_state = LEAVING;
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
		m_ticksToRest = 0;
		// bunked by boulder
		if (amount == 100)
			getWorld()->increaseScore(500);
		// not bunked by boulder
		else
		{
			if (m_hardcore)
				// hardcore protester
				getWorld()->increaseScore(250);
			else
				// regular protester
				getWorld()->increaseScore(100);
		}
	}

}

void Protester::setRestTicks(int ticks)
{
	m_ticksToRest = ticks;
}

RegularProtester::RegularProtester(StudentWorld* sw)
	:Protester(PROTESTER, sw, TID_PROTESTER, 5)
{

}

RegularProtester::~RegularProtester()
{

}

void RegularProtester::bribed()
{
	Protester::bribed();
	getWorld()->increaseScore(25);
	// make it enter leaving state
	decHitPoints(1000);
}

HardcoreProtester::HardcoreProtester(StudentWorld* sw)
	:Protester(PROTESTER, sw, TID_HARD_CORE_PROTESTER, 20, true)
{

}

HardcoreProtester::~HardcoreProtester()
{

}

bool HardcoreProtester::doSomethingSpecial()
{
	int M = 16 + getWorld()->getLevel() * 2;
	int nextX, nextY;
	// if the protester and the tunnelman are separated by a distance greater than M,
	// there is no way the protester can get to the tunnelman within M steps
	if (distanceWith(getWorld()->tunnelManX(), getWorld()->tunnelManY()) > M)
		return false;
	int length = findPathTo(getWorld()->tunnelManX(), getWorld()->tunnelManY(), nextX, nextY);
	if (length >= 0 && length <= M)
	{
		if (nextX == getX() - 1)
			setDirection(left);
		if (nextX == getX() + 1)
			setDirection(right);
		if (nextY == getY() - 1)
			setDirection(down);
		if (nextY == getY() + 1)
			setDirection(up);
		moveOneStep();
		return true;
	}
	return false;
}

void HardcoreProtester::bribed()
{
	Protester::bribed();
	getWorld()->increaseScore(50);
	// stop for some time
	int ticks;
	if (100 - getWorld()->getLevel() * 10 >= 50)
		ticks = (100 - getWorld()->getLevel() * 10);
	else
		ticks = 50;
	setRestTicks(ticks);
}

