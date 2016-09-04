// Included files
#ifndef ENTITY
#define ENTITY
#include"graphics.h"

// The instance of the character class. Only one instance of the character class is used.
#define mycharaacter character::instance()
// Defines the movement speed of the character on screen
#define characterMovementSpeed 400.0f
// Defines the speed of the enemy in the aware and unaware states
#define enemyUnawareSpeed 200.0f
#define enemyAwareSpeed 350.0f

class state;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							     ENTITIES                           *********************///////////////////////

// The entities.h header declares the classes that define the entity framework in the system. Four types of entities: character, enemy, coin 
// and obstacle.

// Acts as the base class for all entities to be derived from.
class baseEntity
{
	int id;				// unique identification
	sf::Vector2f pos;	// position on the screen
	int counter;		// used to assign id
public:
	// constructors
	baseEntity() : id(counter++), pos({ 0, 0 })	{}
	baseEntity(float posx, float posy) : id(counter++), pos({ posx, posy })	{}
	
	// update function to be executed each time step
	virtual void update() = 0;

	// getters and setters
	void setId(int i)	{ id = i; }
	void setPosition(sf::Vector2f position) { pos.x = position.x;	pos.y = position.y; }
	int getId()	{ return id; }
	sf::Vector2f getPosition() { return pos; }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							   MOVABLE ENTITY                     *********************///////////////////////

// Class for a movable entity that is controlled by the state machine system. The enemy entity is currently the only such entity in the system.
class movingEntity : public baseEntity
{
	// data members to control movement
	sf::Vector2f direction;
	float speed;
	
public:
	movingEntity() : baseEntity(), direction({1, 0}), speed(enemyUnawareSpeed){}
	movingEntity(float posx, float posy) : baseEntity(posx, posy), direction({ 1, 0 }), speed(enemyUnawareSpeed)	{}
	
	// getters and setters
	sf::Vector2f getDirection(){ return direction; }
	float getSpeed(){ return speed; }

	void setDirection(sf::Vector2f newDirection){ direction = newDirection; }
	void setSpeed(float newSpeed){ speed = newSpeed; }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							     COIN                           *********************///////////////////////

// Class for the coin entity.
class coin :public baseEntity
{
	coinDrawable *newCoin;	// the drawables defined in graphics.h control the creation of the sprite
public:

	// constructors
	coin() :baseEntity()	{ newCoin = new coinDrawable(); }
	coin(float posx, float posy) : baseEntity(posx, posy)	{ newCoin = new coinDrawable(posx, posy); }
	
	void update()	{}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							     OBSTACLE                           *********************///////////////////////

// Class for the obstacle entity
class obstacle :public baseEntity
{
	obstacleDrawable *newObstacle;	// the drawables defined in graphics.h control the creation of the sprite
public:

	// constructors
	obstacle() : baseEntity() { newObstacle = new obstacleDrawable(); }
	obstacle(float posx, float posy, float width, float height) : baseEntity(posx, posy) 
	{ newObstacle = new obstacleDrawable(posx, posy, height, width); }
	
	void update()	{}
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							     CHARACTER                           *********************///////////////////////
// Class for the character entity. The chcaracter entity allows movement control.
class character :public baseEntity
{
	characterDrawable *newCharacter;		// the sprite to be drawn on the screen.
	int score = 0;							// the score so far. collecting coins gives score.
	void coinCollected(){ score++; awa = true; }	// Called each time a coin is collected, sets score and awareness.
	bool awa = false;						// whether the enemies are aware of the character or not
	sf::Clock clock;						// clock to determine time elapsed for the purpose of movement
	
public:
	// constructors
	character() : baseEntity(), score(0) { newCharacter = new characterDrawable(); }
	character(float posx, float posy) : baseEntity(posx, posy), score(0)
	{
		newCharacter = new characterDrawable(posx, posy);
	}

	// function to return score
	int getScore()	{ return score; }

	// function to handle the movement
	void handleMovement();
	void update();

	// only a single instance of the character is used
	static character* instance();

	// set the position values correctly for the entity position data member and the drawable sprite on the screen
	void setPos(sf::Vector2f newpos)
	{
		setPosition(newpos);
		newCharacter->setPositionOfDrawable(newpos);
	}

	// get awareness value
	bool getAwa(){ return awa; }
	// set awareness value
	void setAwa(bool a){ awa = a; }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							     ENEMY                           *********************///////////////////////

// Class for the enemy entity.

class enemy :public movingEntity
{
	enemyDrawable *newEnemy;
	state *currentState;	// the enemy can be in one of two states, aware and unaware
	
public:
	// constructors
	enemy();
	enemy(float posx, float posy);

	void awareNow(){ newEnemy->gotAngry(); }	// called when the enemy becomes aware of the character
	void notAwareNow(){ newEnemy->notAngry(); }	// called when the enemy is no longer aware 
	sf::Clock clock;		// clock to determince elapsed time for movement
	sf::Clock myTimer;		// timer for the aware state. the enemy stays in aware state for five seconds.
	void changeState(state *newState);			// function to change the enemy state
	void update();

	// set the position values correctly for the entity position data member and the drawable sprite on the screen
	void setPos(sf::Vector2f newPosition) 
	{
		setPosition(newPosition); newEnemy->setPositionOfDrawable(newPosition);
	}
};

#endif