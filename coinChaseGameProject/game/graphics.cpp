#include"graphics.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** sfDraw::loadTextures **********
void sfDraw::loadTextures()
{
	// load the required textures
	textures[characterTex].loadFromFile(".\\textures\\character.png");
	textures[enemyTex].loadFromFile(".\\textures\\enemy.png");
	textures[coinTex].loadFromFile(".\\textures\\coin.png");
	textures[obstacleTex].loadFromFile(".\\textures\\obstacle.png");
	textures[enemyAngry].loadFromFile(".\\textures\\enemyAngry.png");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                      ********** sfDraw constructors **********
sfDraw::sfDraw() : windowWidth(1000), windowHeight(800)
{
	// create window
	window.create(sf::VideoMode(windowWidth,windowHeight),"Coin Chase!");
	window.setKeyRepeatEnabled(false);

	loadTextures();
}

sfDraw::sfDraw(int winWidth, int winHeight, std::string title) :windowHeight(winHeight), windowWidth(winWidth), windowTitle(title)
{
	// create window
	window.create(sf::VideoMode(windowWidth, windowHeight), windowTitle);
	window.setKeyRepeatEnabled(false);
	
	loadTextures();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                ************* sfDraw::update function **************
void sfDraw::update()
{

	// event handling
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}

	// clear
	window.clear(sf::Color::Cyan);

	// draw all the objects that are to be drawn... ie present in the vector
	for (auto shap : drawables)
	{
		window.draw(*(shap->getDrawable()));
	}

	// display
	window.display();
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                   ************** sfDraw::drawNoMore *****************
// stop drawing an entity in further iterations
void sfDraw::drawNoMore(drawableEntity *object)
{
	auto vecit = drawables.begin();
	for (; vecit != drawables.cend(); vecit++)
	{
		if ((*vecit) == object)
			break;
	}
	drawables.erase(vecit);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                     ************** sfDraw::Instance *****************
sfDraw* sfDraw::Instance()
{
	static sfDraw Instance;

	return &Instance;
}


// array defining the possible locations of the coing on the screen
sf::Vector2f possibleLocations[5] = { { 600.0f, 600.0f }, { 50.0f, 700.0f }, { 600.0f, 50.0f }, { 800.0f, 30.0f }, { 600.0f, 200.0f } };
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             ************** coinDrawable::relocate *****************
void coinDrawable::relocate()
{
	currentLocation++;
	setPositionOfDrawable(possibleLocations[currentLocation%5]);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                           ************** function to check collision conditions *****************
inline bool condition(int &x1, int &x2, int &y1, int &y2, int width, int height)
{	// condition to check the overlap of the two sprites
	return (
		(
		((x1 < x2) && (x2 < (x1 + 60)))
		||
		((x2 < x1) && (x1 < (x2 + width)))
		)
		&&
		(
		((y1 < y2) && (y2 < (y1 + 60)))
		||
		((y2 < y1) && (y1 < (y2 + height)))
		)
		);
}

bool collision(drawableEntity *otherEntity, drawableEntity *entity2)
{
	// get the position values of the entities
	int x1 = entity2->getPositionAsVector().x, y1 = entity2->getPositionAsVector().y, x2 = otherEntity->getPositionAsVector().x, y2 = otherEntity->getPositionAsVector().y;
	int oeWidth = otherEntity->getSizeVector().x, oeHeight = otherEntity->getSizeVector().y;
	return condition(x1,x2,y1,y2,oeWidth,oeHeight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         ************** characterDrawable::checkCollision *****************
// Function checks collision of character with other entities and returns the id of the colliding entity.
int characterDrawable::checkCollision()
{
	for (auto &anEntity : drawIns->drawables)	// check with all the drawable entities.
	{
		if (collision(anEntity, this))
		{
			if (anEntity->getid() == co)		// if collision with a coin, move it to another position 
			{
				anEntity->relocate();
				return co;
			}
			else if (anEntity->getid() == en)	// if collision with enemy
				return en;
			else if (anEntity->getid() == ob)	// if collision with obstacle
				return ob;
		}
	}
	return 0;
}