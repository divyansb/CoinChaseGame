#include"states.h"
#include"entities.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** normalize **********
// returns the normalized vector for the supplied vector. returns scaled values in x and y for the supplied vector
void normalize(sf::Vector2f &theVector)
{
	float devisor = abs(theVector.x) + abs(theVector.y) ;
	theVector.x /= devisor;
	theVector.y /= devisor;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** awareState::instance **********
// returns the unique instance of the awareState class.
awareState* awareState::instance()
{
	static awareState instance;
	return &instance;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** awareState::enter **********
void awareState::enter(enemy *theEnemy)
{
	theEnemy->awareNow();			
	theEnemy->myTimer.restart();	// restart aware state timer
	theEnemy->clock.restart();		// restart closk
	theEnemy->setSpeed(enemyAwareSpeed);		// enemy speed in the aware state
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** awareState::execute **********
// While in aware state the enemy seeks out the character on the screen.
void seek(enemy *theEnemy)
{
	float deltaTime = theEnemy->clock.restart().asSeconds();					// time elapsed
	sf::Vector2f dir = mycharaacter->getPosition() - theEnemy->getPosition();	// direction for required motion
	normalize(dir);
	sf::Vector2f dist = theEnemy->getSpeed()*dir*deltaTime;						// distance to travel
	sf::Vector2f newpos = theEnemy->getPosition() + dist;						// new position 
	theEnemy->setPos(newpos);
}

void awareState::execute(enemy *theEnemy)
{
	if (!mycharaacter->getAwa())	// check awareness
		theEnemy->changeState(unaware);
	if (theEnemy->myTimer.getElapsedTime().asSeconds() >= 5)	// check timer, and change awareness after five seconds
	{
		mycharaacter->setAwa(false);
	}

	// enemy movement in the aware state
	seek(theEnemy);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** awareState::exit **********
// to be executed when the state is exited.
void awareState::exit(enemy *theEnemy)
{
	theEnemy->notAwareNow();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** unawareState::instance **********
// returns the unique instance of the unawareState class.
unawareState* unawareState::instance()
{
	static unawareState instance;
	return &instance;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** unawareState::enter **********
void unawareState::enter(enemy *theEnemy)
{
	theEnemy->setSpeed(enemyUnawareSpeed);	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** unawareState::execute **********
// While in unaware state, the enemy wanders around.
void wander(enemy *theEnemy)
{
	float deltaTime = theEnemy->clock.restart().asSeconds();	// elapsed time
	sf::Vector2f dir = theEnemy->getDirection();				// current movement direction
	int option = rand() % 3;
	switch (option)												// choose a random new direction close to the current direction
	{
	case 0: theEnemy->setDirection({ (dir.x + 0.01f), (dir.y - 0.01f) }); break;
	case 1: theEnemy->setDirection({ (dir.x - 0.01f), (dir.y + 0.01f) }); break;
	case 2: break;
	}

	normalize(dir);
	sf::Vector2f dist = theEnemy->getSpeed()*dir*deltaTime;		// distance to travel
	sf::Vector2f newpos = theEnemy->getPosition() + dist;		// new position
	if (newpos.x >= 920 || newpos.x <= 0 || newpos.y >= 720 || newpos.y <= 0)	// do not go out of the screen
		theEnemy->setDirection((-1.0f)*dir);
	theEnemy->setPos(newpos);
}

void unawareState::execute(enemy *theEnemy)
{

	if (mycharaacter->getAwa())
		theEnemy->changeState(aware);

	// enemy movement for the unaware state
	wander(theEnemy);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** unawareState::exit **********
// to be executed when the state is exited.
void unawareState::exit(enemy *theEnemy)
{
	;
}
