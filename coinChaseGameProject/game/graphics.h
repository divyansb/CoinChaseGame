// Header files required
#include<SFML\System.hpp>
#include<SFML\Window.hpp>
#include<SFML\Graphics.hpp>
#include<SFML\Audio.hpp>
#include<iostream>


// The purpose of the classes in this module is to deal with graphics and drawables pertinent to the main game.
// It also deals with detection of overlap of multiple sprites to signiy collision.

// Macro declarations 
#define drawIns sfDraw::Instance()
#define numTextures 5
// Enumeration to access texture array with greater readability
enum tex{ characterTex, enemyTex, coinTex, obstacleTex, enemyAngry };
// Enumeration to determine the type of the entity
enum id{ch,en,co,ob};
// Class declaration 
// required by class sfDraw having a vector to pointers of drawableEntity
class drawableEntity;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							     SF DRAW                           *********************///////////////////////

// Singleton class used to deal and interact with SFML. It stores all required textures and all drawable objects. Has the main graphics update
// function to be called in each iteration of the main game loop.

class sfDraw
{
	// Data members
	int windowHeight, windowWidth;				// window dimensions
	std::string windowTitle;					// title of the window
	sf::RenderWindow window;					// the window object
	sf::Texture textures[numTextures];			// array storing all the textures required by the game

public:
	std::vector<drawableEntity*> drawables;		// vector of drawableEntity that have been declared so far and need to be drawn evry step
	
	// constructors
	sfDraw();
	sfDraw(int winWidth, int winHeight, std::string title);
	inline void loadTextures();	// small function to load all possible textures

	// function to render all drawableEntites and handle window events
	void update();

	// return draw instance
	static sfDraw* Instance();
	
	// return appropriate texture based on the index value passed
	sf::Texture& getTexture(int index) { return textures[index]; }

	// add object to the vector of drawable entities
	void addObject(drawableEntity *object)
	{
		drawables.push_back(object);
	}

	// is window open?
	bool isWindowOpen() { return window.isOpen(); }

	// function to close window and end game
	void end()	{ window.close(); }

	// function to delete an object and stop drawing it in further cycles
	void drawNoMore(drawableEntity *object);		

	// return window width and height
	int getWindowHeight(){ return windowHeight; }
	int getWindowWidth(){ return windowWidth; }

};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							DRAWABLE ENTITY                        *********************///////////////////////

// Class that acts as the base for all drawable entities in the project.

class drawableEntity 
{
protected:
	// Members 
	sf::Drawable *obb;			// pointer to the drawable object
	sf::Texture &texture;		// the texture
	sf::Vector2f pos;			// position in the world
	bool tag = false;			// identifies objects that are controlled by player
	int id;						// id to determine the type of the entity
	sf::Vector2f sizeVec;		// the size of the entity on screen

public:
	// Member Functions 

	// constructors
	// initialize the data members and add pointer to this object to the vector of objects to be drawn
	drawableEntity(sf::Drawable *object, int index, float width, float height) : texture(drawIns->getTexture(index)), obb(object), sizeVec({ width, height })
	{
		drawIns->addObject(this); 
	}
	drawableEntity(sf::Drawable *object, float x, float y, int index, float width, float height) : texture(drawIns->getTexture(index)), pos({ x, y }), obb(object), sizeVec({width, height})
	{
		drawIns->addObject(this);
	}

	// Function to return the position of the entity
	sf::Vector2f getPositionAsVector()
	{
		return pos;
	}

	// Return the drawable(to help draw function)
	sf::Drawable* getDrawable()
	{
		return obb;
	}

	// function to move the entity to another position(implemented only by coin at the moment)
	virtual void relocate()	{}

	// is the entity controllable by the player
	bool isControllable() { return tag; }

	// return the id of the entity
	int getid() { return id; }

	// return size vector
	sf::Vector2f getSizeVector()	{ return sizeVec; }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							  CHARACTER                            *********************///////////////////////

// The class holding the details of the character to be drawn. Character can be controlled with the 'wasd' keys; as defined by the handle 
// movement function.

class characterDrawable :public drawableEntity
{
	// Members
	sf::Sprite sprite;

public:
	// Member functions
	// constructors
	characterDrawable() :drawableEntity(&sprite, characterTex, 60, 60)
	{
		sprite.setTexture(texture);
		tag = true;
		id = ch;
	}
	characterDrawable(float x, float y) : drawableEntity(&sprite, x, y, characterTex, 60, 60)
	{
		sprite.setPosition(x,y);
		sprite.setTexture(texture);
		tag = true;
		id = ch;
	}

	// function to set the position value of the character in order to allow movement control.
	void setPositionOfDrawable(sf::Vector2f position)
	{
		pos = position; 
		sprite.setPosition(pos);
	}

	// function to check the collision of the character with other entities
	int checkCollision();

};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							     ENEMY                               *********************///////////////////////

// Enemy class.

class enemyDrawable : public drawableEntity
{
	// Members
	sf::Sprite sprite;

public:
	// Member functions
	// constructors
	enemyDrawable() : drawableEntity(&sprite, enemyTex, 80, 80)
	{
		sprite.setTexture(texture);
		id = en;
	}

	enemyDrawable(float x, float y) : drawableEntity(&sprite, x, y, enemyTex, 80, 80)
	{
		sprite.setPosition(x,y);
		sprite.setTexture(texture);
		id = en;
	}

	// to change the appearance of the enemy
	void gotAngry()
	{
		sprite.setTexture(drawIns->getTexture(enemyAngry));
	}

	// to change the appearance of the enemy
	void notAngry()
	{
		sprite.setTexture(drawIns->getTexture(enemyTex));
	}

	// function to set the position value for the enemy
	void setPositionOfDrawable(sf::Vector2f position)
	{
		pos = position;
		sprite.setPosition(pos);
	}

};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							      COIN                             *********************///////////////////////

// Coin class.

class coinDrawable : public drawableEntity
{
	// Members
	sf::Sprite sprite;	
	int currentLocation;	// the coin can be at one of the several predefined locations stored in an array
	
public:
	// Member functions
	// constructors
	coinDrawable() : drawableEntity(&sprite, coinTex, 60, 60), currentLocation(0)
	{
		sprite.setPosition(600.0f, 600.0f);
		sprite.setTexture(texture);
		id = co;
	}

	coinDrawable(float x, float y) : drawableEntity(&sprite, x, y, coinTex, 60, 60), currentLocation(0)
	{
		sprite.setPosition(x,y);
		sprite.setTexture(texture);
		id = co;
	}

	// function to move the coin to a new position
	void relocate();
	
	// function to set the position velue for the coin
	void setPositionOfDrawable(sf::Vector2f position)
	{
		pos = position;
		sprite.setPosition(pos);
	}


};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////************************							   OBSTACLE                            *********************///////////////////////

class obstacleDrawable : public drawableEntity
{
	// Members
	sf::RectangleShape shape;
	
public:
	// Member functions
	obstacleDrawable() : drawableEntity(&shape, obstacleTex, 100, 100)
	{
		shape.setSize(sizeVec);
		shape.setTexture(&texture);
		id = ob; 
	}

	obstacleDrawable(float x, float y, float width, float height) : drawableEntity(&shape, x, y, obstacleTex, width, height)
	{
		shape.setSize(sizeVec);
		shape.setPosition(x,y);
		shape.setTexture(&texture);
		id = ob; 
	}


};