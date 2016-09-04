// Header files
#include<iostream>
#include"entities.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         ************** createTutorial *****************
// Function to create the tutorial window.
void createTutorial()
{
	// create a window and display tutorial until a key is pressed.
	sf::RenderWindow tutorialWindow;
	tutorialWindow.create(sf::VideoMode(800, 800), "Coin Chase!");
	sf::Sprite tutorial;	// the tutorial is displayed as a sprite on screen
	tutorial.setPosition(50, 50);
	sf::Texture tutTex;
	tutTex.loadFromFile(".\\textures\\tutorial.png");
	tutorial.setTexture(tutTex);
	sf::Event event;
	while (tutorialWindow.isOpen())
	{
		while (tutorialWindow.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
				tutorialWindow.close();
		}
		tutorialWindow.draw(tutorial);
		tutorialWindow.display();
	}

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         ************** createEndScreen *****************
// Function creates the game end screen.
void createEndScreen(int score)
{
	// create a window displayin the score nd close it when q is pressed.
	std::string s = "Game Over! \npress q to quit\n\nYour score : " + std::to_string(score);
	sf::RenderWindow endScreen;
	endScreen.create(sf::VideoMode(800,600),"Game Over!");
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");
	sf::Text text;	text.setFont(font);
	text.setPosition(50.0f,50.0f);
	text.setString(s);
	text.setColor(sf::Color::Yellow);
	text.setCharacterSize(60);
	sf::Event event;
	while (endScreen.isOpen())
	{
		while (endScreen.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Q)
				endScreen.close();
		}

		endScreen.draw(text);
		endScreen.display();
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                         ************** main function *****************
int main()
{
	createTutorial();

		enemy steven(400, 400);
		coin c1(600, 600);
		obstacle ob1(150, 100, 200, 100);
		obstacle ob2(750, 100, 200, 100);
		obstacle ob3(150, 500, 200, 100);
		obstacle ob4(750, 500, 200, 100);

		while (drawIns->isWindowOpen())
		{
			steven.update();
			mycharaacter->update();
			drawIns->update();
		}

		createEndScreen(mycharaacter->getScore());

	return 0;
}