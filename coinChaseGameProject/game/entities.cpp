#include"entities.h"
#include"states.h"
#include<iostream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** character::update **********
// to be called in each update step.
void character::update()
{
	handleMovement();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** character::handleMovement **********
// function to handle the movement of the character using the wasd keys.
void character::handleMovement()
{
	float time = clock.restart().asSeconds();	// get elapsed time in each iteration
	switch (newCharacter->checkCollision())		// check collision
	{
	case en: drawIns->end(); break;		// collision with enemy => game end
	case co: coinCollected(); break;	// collision with coin => coin collected
	case ob: drawIns->end(); break;		// collision with obstacle => game end
	}
	    sf::Vector2f position = newCharacter->getPositionAsVector();	// character position
		// change position based on the key pressed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && position.x < (drawIns->getWindowWidth() - 60))
			position.x += characterMovementSpeed * time;	// move right with D
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && position.x > 0)
			position.x -= characterMovementSpeed * time;	// move left with A
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && position.y < (drawIns->getWindowHeight() - 60))
			position.y += characterMovementSpeed * time;	// move down with S
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && position.y > 0)
			position.y -= characterMovementSpeed * time;	// move up with W
		// set new position value
		setPos(position);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** character::instance **********
// returns the unique instance of the character
character* character::instance()
{
	static character instance;
	return &instance;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** enemy::update **********
void enemy::update()
{
	currentState->execute(this);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** enemy::constructors **********
enemy::enemy() : movingEntity(), currentState(unaware)
{ newEnemy = new enemyDrawable(); }

enemy::enemy(float posx, float posy) : movingEntity(posx, posy), currentState(unaware) 
{ newEnemy = new enemyDrawable(posx, posy); }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** enemy::changeState **********
// used by the enemy entity to change the current state.
void enemy::changeState(state *newState)
{
	currentState->exit(this);
	currentState = newState;
	currentState->enter(this);
}
