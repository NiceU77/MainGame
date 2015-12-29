#include "menu.h"

void MainMenu::InitMenu(Texture& menuTexture, Font& font)
{
	menuSprite.setTexture(menuTexture);

	startText.setFont(font);
	startText.setCharacterSize(28);
	startText.setRotation(355);
	ostringstream toString;
	toString << "Start Game";
	startText.setString(toString.str());
	startText.setOrigin(startText.getGlobalBounds().width / 2, startText.getGlobalBounds().height / 2);

	exitText.setFont(font);
	exitText.setCharacterSize(28);
	exitText.setRotation(355);
	ostringstream toString1;
	toString1 << "Exit";
	exitText.setString(toString1.str());
	exitText.setOrigin(exitText.getGlobalBounds().width / 2, exitText.getGlobalBounds().height / 2);

}

void MainMenu::Update(int& volume, View& view)
{
	startText.setPosition(view.getCenter().x - START_GAME_TEXT_SHIFT.x, view.getCenter().y - START_GAME_TEXT_SHIFT.y);
	exitText.setPosition(view.getCenter().x - EXIT_GAME_TEXT_SHIFT.x, view.getCenter().y - EXIT_GAME_TEXT_SHIFT.y);

	menuSprite.setPosition(0, 0);
	menuSprite.setScale(2, 2);
}

void MainMenu::CheckCollisionWithMouse(RenderWindow& window, GameState& gameState, Event& event)
{
	Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	FloatRect startTextBounds = startText.getGlobalBounds();
	FloatRect exitTextBounds = exitText.getGlobalBounds();
	if (startTextBounds.contains(mouse))
	{
		startText.setColor(COLOR_WHILE_MOUSE_ON_TEXT);
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			gameState = GAME;
		}
	}
	else if (exitTextBounds.contains(mouse))
	{
		exitText.setColor(COLOR_WHILE_MOUSE_ON_TEXT);
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			window.close();
		}
	}
	else
	{
		startText.setColor(Color::White);
		exitText.setColor(Color::White);
	}
}

void MainMenu::Draw(RenderWindow& window, GameState& gameState, Event& event)
{
	CheckCollisionWithMouse(window, gameState, event);
	window.draw(menuSprite);
	window.draw(startText);
	window.draw(exitText);
}