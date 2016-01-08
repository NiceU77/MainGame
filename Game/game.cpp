#include "game.h"

void Game::InitEnemies()
{
	enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(FLY1_POSITION_X - 25, FLY1_POSITION_Y), FLY_SIZE.x, FLY_SIZE.y, "EnemyFly", 1, 1));
	enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(FLY2_POSITION_X - 25, FLY2_POSITION_Y), FLY_SIZE.x, FLY_SIZE.y, "EnemyFly", 1, 1));
	enemies.push_back(Enemy(mySprites.standAndShootTexture, Vector2f(1400, 200), 38, 43, "EnemyStandAndShoot", 3, 2));
	enemies.push_back(Enemy(mySprites.standAndShootTexture, Vector2f(1500, 200), 38, 43, "EnemyStandAndShoot", 3, 2));
	enemies.push_back(Enemy(mySprites.standAndShootTexture, Vector2f(1300, 300), 38, 43, "EnemyStandAndShoot", 3, 2));
	enemies.push_back(Enemy(mySprites.standAndShootTexture, Vector2f(2100, 300), 38, 43, "EnemyStandAndShoot", 3, 3));
	enemies.push_back(Enemy(mySprites.standAndShootTexture, Vector2f(2600, 300), 38, 43, "EnemyStandAndShoot", 3, 3));
	enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(1500, 900), FLY_SIZE.x, FLY_SIZE.y, "EnemyFly", 1, 5));
	enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(1200, 1050), FLY_SIZE.x, FLY_SIZE.y, "EnemyFly", 1, 5));
	enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(2100, 950), FLY_SIZE.x, FLY_SIZE.y, "EnemyFly", 1, 6));
	enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(1500, 800), FLY_SIZE.x, FLY_SIZE.y, "EnemyFly", 1, 5));
	enemies.push_back(Enemy(mySprites.heroTexture, Vector2f(400, 300), 32, 32, "EnemyFollow", 2, 1));
}

void Game::InitGame()
{
	lastShootPlayer = 0;
	hitTimer = 0;
	//room = 1;
	volume = 30;
	gameState = MAIN_MENU;
	InitEnemies();
	player = Player(mySprites.heroTexture, Vector2f(PLAYER_POSITION_X - 100, PLAYER_POSITION_Y + 700), PLAYER_WIDTH, PLAYER_HEIGHT, "Hero", 6, mySprites.headTexture);
	view.reset(FloatRect(0, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT));
	myTileMap.initMap(myMap);
	mySprites.InitImages();
	mySprites.LoadFont();
	mySounds.LoadMusic();
	menu.InitMenu(mySprites.mainMenuTexture, mySprites.font);
	isKeyPressed = false;
	mySounds.menuMusic.play();
}

void Game::Delete()
{
	myMap.clear();
	enemies.clear();
	bullets.clear();
	chests.clear();
	gameTimer.restart();
	clock.restart();
	boomb.createTime = 0;
}

void Game::Restart()
{
	Delete();
	lastShootPlayer = 0;
	hitTimer = 0;
	room = 1;
	volume = 30;
	gameState = MAIN_MENU;
	InitEnemies();
	player = Player(mySprites.heroTexture, Vector2f(PLAYER_POSITION_X - 100, PLAYER_POSITION_Y + 700), PLAYER_WIDTH, PLAYER_HEIGHT, "Hero", 6, mySprites.headTexture);
	view.reset(FloatRect(0, float(WINDOW_HEIGHT), float(WINDOW_WIDTH), float(WINDOW_HEIGHT)));
	myTileMap.initMap(myMap);
	mySprites.InitImages();
	mySounds.LoadMusic();
	menu.InitMenu(mySprites.mainMenuTexture, mySprites.font);
	isKeyPressed = false;
	mySounds.menuMusic.play();
}

int Game::InitializeRoom()
{
	if (player.position.x > 0 && player.position.x < WINDOW_WIDTH)
	{
		if (player.position.y > 0 && player.position.y < WINDOW_HEIGHT)
		{
			return FIRST;
		}
		else if (player.position.y > WINDOW_HEIGHT && player.position.y < WINDOW_HEIGHT * 2)
		{
			return FORTH;
		}
		else if (player.position.y > WINDOW_HEIGHT * 2 && player.position.y < WINDOW_HEIGHT * 3)
		{
			return SEVENTH;
		}
	}
	else if (player.position.x > WINDOW_WIDTH && player.position.x < WINDOW_WIDTH * 2)
	{
		if (player.position.y > 0 && player.position.y < WINDOW_HEIGHT)
		{
			return SECOND;
		}
		else if (player.position.y > WINDOW_HEIGHT && player.position.y < WINDOW_HEIGHT * 2)
		{
			return FIFTH;
		}
		else if (player.position.y > WINDOW_HEIGHT * 2 && player.position.y < WINDOW_HEIGHT * 3)
		{
			return EIGHTH;
		}
	}
	else if (player.position.x > WINDOW_WIDTH * 2 && player.position.x < WINDOW_WIDTH * 3)
	{
		if (player.position.y > 0 && player.position.y < WINDOW_HEIGHT)
		{
			return THIRD;
		}
		else if (player.position.y > WINDOW_HEIGHT && player.position.y < WINDOW_HEIGHT * 2)
		{
			return SIXTH;
		}
		else if (player.position.y > WINDOW_HEIGHT * 2 && player.position.y < WINDOW_HEIGHT * 3)
		{
			return NINTH;
		}
	}
	return 0;
}

void Game::SetPauseText()
{
	continueText.setString("CONTINUE");
	continueText.setFont(mySprites.font);
	continueText.setCharacterSize(28);
	continueText.setRotation(5);
	continueText.setOrigin(continueText.getGlobalBounds().width / 2, continueText.getGlobalBounds().height / 2);
	continueText.setPosition(view.getCenter().x + 5, view.getCenter().y - 120);

	exitText.setString("EXIT");
	exitText.setFont(mySprites.font);
	exitText.setCharacterSize(28);
	exitText.setRotation(5);
	exitText.setOrigin(exitText.getGlobalBounds().width / 2, exitText.getGlobalBounds().height / 2);
	exitText.setPosition(view.getCenter().x - 10, view.getCenter().y + 100);

	menuText.setString("GO TO MENU");
	menuText.setFont(mySprites.font);
	menuText.setCharacterSize(28);
	menuText.setRotation(5);
	menuText.setOrigin(menuText.getGlobalBounds().width / 2, menuText.getGlobalBounds().height / 2);
	menuText.setPosition(view.getCenter().x, view.getCenter().y + 40);
}

void Game::CheckIntersectionWithTextPause(RenderWindow& window)
{
	Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (continueText.getGlobalBounds().contains(mousePos))
	{
		continueText.setColor(COLOR_WHILE_MOUSE_ON_TEXT);
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			gameState = GAME;
		}
	}
	else if (menuText.getGlobalBounds().contains(mousePos))
	{
		menuText.setColor(COLOR_WHILE_MOUSE_ON_TEXT);
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			gameState = MAIN_MENU;
		}
	}
	else if (exitText.getGlobalBounds().contains(mousePos))
	{
		exitText.setColor(COLOR_WHILE_MOUSE_ON_TEXT);
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			window.close();
		}
	}
	else
	{
		continueText.setColor(Color::White);
		menuText.setColor(Color::White);
		exitText.setColor(Color::White);
	}
}

void Game::InitSheetAndBackground(RenderWindow& window)
{
	pauseRect.setFillColor(Color(0, 0, 0, 150));
	pauseRect.setSize(Vector2f(view.getSize().x, view.getSize().y));
	pauseRect.setOrigin(pauseRect.getSize().x / 2, pauseRect.getSize().y / 2);
	pauseRect.setPosition(view.getCenter());

	pauseSprite.setTexture(mySprites.pauseTexture);
	pauseSprite.setOrigin(pauseSprite.getGlobalBounds().width / 2, pauseSprite.getGlobalBounds().height / 2);
	pauseSprite.setPosition(view.getCenter());
}

void Game::SetPause(RenderWindow& window)
{
	InitSheetAndBackground(window);
	SetPauseText();
	CheckIntersectionWithTextPause(window);
}

void Game::SetEndGameText(RenderWindow& window)
{
	if (gameState == END_GAME)
	{
		continueText.setString("TRY AGAIN");
	}
	else
	{
		continueText.setString("WIN\n\nGO AGAIN");
	}
	continueText.setFont(mySprites.font);
	continueText.setCharacterSize(35);
	continueText.setRotation(5);
	continueText.setOrigin(continueText.getGlobalBounds().width / 2, continueText.getGlobalBounds().height / 2);
	continueText.setPosition(view.getCenter().x + 5, view.getCenter().y - 100);

	exitText.setString("YES");
	exitText.setFont(mySprites.font);
	exitText.setCharacterSize(28);
	exitText.setRotation(5);
	exitText.setOrigin(exitText.getGlobalBounds().width / 2, exitText.getGlobalBounds().height / 2);
	exitText.setPosition(view.getCenter().x - 50, view.getCenter().y + 40);

	menuText.setString("NO");
	menuText.setFont(mySprites.font);
	menuText.setCharacterSize(28);
	menuText.setRotation(5);
	menuText.setOrigin(menuText.getGlobalBounds().width / 2, menuText.getGlobalBounds().height / 2);
	menuText.setPosition(view.getCenter().x + 50, view.getCenter().y + 40);
}

void Game::CheckIntersectionWithTextEnd(RenderWindow& window)
{
	Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (menuText.getGlobalBounds().contains(mousePos))
	{
		menuText.setColor(COLOR_WHILE_MOUSE_ON_TEXT);
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			Restart();
			gameState = MAIN_MENU;
		}
	}
	else if (exitText.getGlobalBounds().contains(mousePos))
	{
		exitText.setColor(COLOR_WHILE_MOUSE_ON_TEXT);
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			Restart();
			gameState = GAME;
		}
	}
	else
	{
		menuText.setColor(Color::White);
		exitText.setColor(Color::White);
	}
}

void Game::SetEndGame(RenderWindow& window)
{
	InitSheetAndBackground(window);
	SetEndGameText(window);
	CheckIntersectionWithTextEnd(window);
}

void Game::DeleteEnemyFromVector()
{
	auto isDead = [](Enemy enemy)
	{
		return (enemy.alive == false);
	};
	enemies.erase(remove_if(enemies.begin(), enemies.end(), isDead), enemies.end());
}

void Game::EnemyDeathSound(Enemy& enemy)
{
	if (enemy.alive == false && enemy.currentFrame == 0)
	{
		if (enemy.name == "EnemyFly")
		{
			mySounds.flyHurt.play();
		}
		else
		{
			mySounds.enemyHurt.play();
		}
	}
}

void Game::UpdateEnemies(RenderWindow& window)
{
	DeleteEnemyFromVector();
	for (auto& enemy: enemies)
	{
		EnemyDeathSound(enemy);
		if (enemy.alive == true)
		{
			if (room == enemy.enemyRoom)
			{
				enemy.MoveFollowEnemy(gameTime, player.position, myMap, time);
				enemy.UpdateFly(time, myMap, mySprites.wallBackgroundSprite);
				enemy.UpdateStandAndShoot(bullets, gameTime);
				enemy.Update(boomb, gameTime);
			}
			enemy.ExplosionCollision(boomb, gameTime);
			enemy.deathTime = gameTime;
		}
	}
}

void Game::UpdatePlayer()
{
	player.Moving(time, myMap, view, IsRoomCleared(), mySprites.wallBackgroundSprite);
	player.CheckEnemyCollidesPlayer(enemies, gameTime, hitTimer, mySounds.playerHurts);
	player.ChangeColorAfterHit(gameTime, hitTimer);
	player.CheckExplosionCollision(boomb, gameTime, mySounds.playerHurts);
}

void Game::UpdateChests(RenderWindow& window)
{
	for (auto& chest: chests)
	{
		chest.Update(player, mySounds.chestOpening);
	}
}


void Game::DeleteBulletFromVector()
{
	auto isDead = [](Bullet bullet) 
	{
		return (bullet.deathTime > bullet.deathTime + BULLET_ANIMATION_STEP_TIME * 2);
	};
	bullets.erase(remove_if(bullets.begin(), bullets.end(), isDead), bullets.end());
}

void Game::UpdatePlayersBullets(Bullet& bullet)
{
	for (auto& enemy: enemies)
	{
		if (enemy.health > 0 && bullet.isPlayers == true)
		{
			if (Collision::PixelPerfectTest(enemy.sprite, bullet.bulletSprite))
			{
				bullet.deathTime = gameTime;
				bullet.alive = false;
				enemy.health -= bullet.damage;
				enemy.playerHitTime = gameTime;
				mySounds.tearDestroy.play();
			}
		}
	}
}

void Game::UpdateEnemiesBullets(Bullet& bullet)
{
	if (bullet.isPlayers == false)
	{
		if (Collision::PixelPerfectTest(player.sprite, bullet.bulletSprite) || Collision::PixelPerfectTest(player.headSprite, bullet.bulletSprite))
		{
			bullet.alive = false;
			player.health -= bullet.damage;
			player.lastHitTime = gameTime;
			mySounds.playerHurts.play();
			mySounds.tearDestroy.play();		
		}
	}
}

void Game::UpdateBullets(RenderWindow& window)
{
	for (auto& bullet: bullets)
	{
		if (bullet.alive == true)
		{
			if (gameState == GAME)
			{
				UpdatePlayersBullets(bullet);
				UpdateEnemiesBullets(bullet);
				bullet.UpdateBullet(time, window, gameTime, mySprites.bulletTexture, mySprites.bulletEffectTexture, mySprites.bulletEffectTextureEnemy);
				bullet.CheckCollisionBullet(gameTime, myMap, mySprites.wallBulletSprite, mySounds.tearDestroy);
				bullet.DeleteBullet(gameTime, mySounds.tearDestroy);
			}
			bullet.DrawBullet(window);
		}
		else
		{
			bullet.BulletDestroyEffect(gameTime, window);
		}
	}
	DeleteBulletFromVector();
}

void Game::UpdateBombs()
{
	
	boomb.Update(gameTime);
	boomb.PlaySound(mySounds.bombExplosion, gameTime);
	for (auto& map : myMap)
	{
		if (map.sprite.getGlobalBounds().intersects(boomb.damageZone.getGlobalBounds()) && map.pos == 0)
		{
			map.alive = false;
		}
	}
	auto isDestroyed = [](Map map)
	{
		return (map.alive == false);
	};
	myMap.erase(remove_if(myMap.begin(), myMap.end(), isDestroyed), myMap.end());
}

void Game::UpdateTime()
{
	if (player.health > 0)
	{
		gameTime = gameTimer.getElapsedTime().asSeconds();
	}
}

void Game::UpdateTimePerFrame()
{
	time = float(clock.getElapsedTime().asMicroseconds());
	clock.restart();
	time = time / GAME_TIME_DIFFERENCE;
}

void Game::UpdateSounds()
{
	mySounds.UpdateMusic(volume);
}

void Game::ProcessEvents(RenderWindow& window)
{
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
	}
	if (gameState == GAME)
	{
		player.Control(boomb, bullets, time, gameTime, lastShootPlayer, mySounds.tearFire);
	}
}

void Game::UpdatePause()
{
	if (Keyboard::isKeyPressed(Keyboard::Escape) && isKeyPressed == false)
	{
		isKeyPressed = true;
		if (gameState == GAME)
		{
			gameState = PAUSE;
		}
		else
		{
			gameState = GAME;
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::Escape) == false)
	{
		isKeyPressed = false;
	}
}

void Game::SetMainMenuMusic()
{
	mySounds.backgroundMusic1.stop();
	mySounds.backgroundMusic2.stop();
	mySounds.backgroundMusic3.stop();
	if (mySounds.menuMusic.getStatus() == false)
	{
		mySounds.menuMusic.play();
	}
}

void Game::CheckEndGame()
{
	if (player.health <= 0)
	{
		gameState = END_GAME;
	}
	if (enemies.size() == 0)
	{
		gameState = FINISHED;
	}
}

void Game::UpdateGame(RenderWindow& window)
{
	UpdatePause();
	UpdateTimePerFrame();
	if (gameState == MAIN_MENU)
	{
		SetMainMenuMusic();
		menu.Update(volume, view);
	}
	else if (gameState == GAME)
	{
		mySounds.menuMusic.stop();
		AddChest(view);
		UpdatePlayer();
		UpdateTime();
		UpdateEnemies(window);
		UpdateChests(window);
		UpdateBombs();
		UpdateSounds();
		CheckEndGame();
		room = InitializeRoom();
	}
	else
	{
		
		boomb.damageZone.setPosition(-TILE_SIDE * 2, -TILE_SIDE * 2);
		SetEndGame(window);
	}
	if (window.hasFocus())
	{
		ProcessEvents(window);
	}
}

void Game::DrawBackground(RenderWindow& window)
{
	mySprites.wallBackgroundSprite.setOrigin(mySprites.wallBackgroundSprite.getGlobalBounds().width / 2, mySprites.wallBackgroundSprite.getGlobalBounds().height / 2);
	mySprites.wallBackgroundSprite.setPosition(view.getCenter().x, view.getCenter().y);
	mySprites.floorBackgroundSprite.setOrigin(mySprites.floorBackgroundSprite.getGlobalBounds().width / 2, mySprites.floorBackgroundSprite.getGlobalBounds().height / 2);
	mySprites.floorBackgroundSprite.setPosition(view.getCenter().x, view.getCenter().y);
	mySprites.wallBulletSprite.setOrigin(mySprites.wallBulletSprite.getGlobalBounds().width / 2, mySprites.wallBulletSprite.getGlobalBounds().height / 2);
	mySprites.wallBulletSprite.setPosition(view.getCenter().x, view.getCenter().y);

	window.draw(mySprites.wallBackgroundSprite);
	window.draw(mySprites.floorBackgroundSprite);
	window.draw(mySprites.wallBulletSprite);
}

void Game::DrawBombCount(RenderWindow& window)
{
	Sprite bombCountSprite;
	bombCountSprite.setTexture(mySprites.bombCount);
	bombCountSprite.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + BOMB_IMAGE_SHIFT.x, view.getCenter().y - WINDOW_HEIGHT / 2 + BOMB_IMAGE_SHIFT.y);
	bombCountSprite.setScale(BOMB_IMAGE_SCALE, BOMB_IMAGE_SCALE);
	window.draw(bombCountSprite);

	Text textBombs;
	textBombs.setFont(mySprites.font);
	textBombs.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + TEXT_SHIFT.x, view.getCenter().y - WINDOW_HEIGHT / 2 + TEXT_SHIFT.y);
	textBombs.setCharacterSize(FONT_INTERFACE_SIZE);

	textBombs.setString(to_string(player.bombCount));

	window.draw(textBombs);
}

void Game::DrawPlayersHealth(RenderWindow& window)
{
	Sprite fullHP;
	fullHP.setTexture(mySprites.heartTexture);
	fullHP.setTextureRect(IntRect(0, 0, HEALTH_TEXTURE_SIZE, HEALTH_TEXTURE_SIZE));
	fullHP.setScale(HP_SCALE, HP_SCALE);
	Sprite halfHP;
	halfHP.setTexture(mySprites.heartTexture);
	halfHP.setTextureRect(IntRect(HEALTH_TEXTURE_SIZE, 0, HEALTH_TEXTURE_SIZE, HEALTH_TEXTURE_SIZE));
	halfHP.setScale(HP_SCALE, HP_SCALE);
	Sprite emptySprite;
	emptySprite.setTexture(mySprites.heartTexture);
	emptySprite.setTextureRect(IntRect(HEALTH_TEXTURE_SIZE * 2, 0, HEALTH_TEXTURE_SIZE, HEALTH_TEXTURE_SIZE));
	emptySprite.setScale(HP_SCALE, HP_SCALE);

	for (int i = 1; i <= MAX_PLAYER_HEALTH; i++)
	{
		emptySprite.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + i * HP_SHIFT.x, view.getCenter().y - WINDOW_HEIGHT / 2 + HP_SHIFT.y);
		window.draw(emptySprite);
	}
	if (player.health > 0)
	{
		for (int i = 1; i <= player.health; i++)
		{
			fullHP.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + i * HP_SHIFT.x, view.getCenter().y - WINDOW_HEIGHT / 2 + HP_SHIFT.y);
			window.draw(fullHP);
		}
		if (player.health - int(player.health) != 0)
		{
			halfHP.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + int(player.health + 1) * HP_SHIFT.x, view.getCenter().y - WINDOW_HEIGHT / 2 + HP_SHIFT.y);
			window.draw(halfHP);
		}
	}
}

void Game::DrawEnemies(RenderWindow& window)
{
	for (auto& enemy: enemies)
	{
		if (enemy.alive == true && enemy.enemyRoom == room)
		{
			window.draw(enemy.sprite);
		}
		else
		{
			enemy.DestroyEffect(gameTime, window, mySprites.poofTexture, time);
		}
	}
}

void Game::DrawPlayer(RenderWindow& window)
{
	if (player.health > 0)
	{
		window.draw(player.sprite);
		window.draw(player.headSprite);
	}
}

void Game::DrawBombs(RenderWindow& window)
{
	boomb.Draw(window, mySprites.bombState, mySprites.bombExplosion, gameTime, time);	
}

bool Game::IsRoomCleared()
{
	bool isAllDead = true;

	for (auto& enemy: enemies)
	{
		if (enemy.enemyRoom == room && enemy.health > 0)
		{
			return false;
		}
	}
	if (myTileMap.isOpened == false)
	{
		mySounds.doorOpening.play();
	}
	return true;
}

bool Game::IsRoomEmpty()
{
	bool isEmpty = true;

	for (auto& enemy : enemies)
	{
		if (enemy.enemyRoom == room)
		{
			isEmpty = false;
		}
	}
	return isEmpty;
}

bool Game::IsChestInRoom()
{
	bool isChestInRoom = false;

	for (auto& chest: chests)
	{
		if (room == chest.room)
		{
			isChestInRoom = true;
		}
	}
	return isChestInRoom;
}

void Game::AddChest(View& view)
{
	if (IsRoomCleared() && !IsRoomEmpty())
	{
		if (IsChestInRoom() == false)
		{
			chests.push_back(Chest(view.getCenter().x, view.getCenter().y, room));
			mySounds.chestDrop.play();
		}
	}
}

void Game::DrawMap(RenderWindow& window)
{
	myTileMap.drawTiles(myMap, window, IsRoomCleared());
}

void Game::DrawChest(RenderWindow& window)
{
	for (auto& chest: chests)
	{
			chest.DrawChest(window);
	}
}

void Game::SetCorrectDrawOrder(RenderWindow& window)
{
	if (player.position.y + player.h < boomb.position.y + TILE_SIDE)
	{
	    DrawPlayer(window);
		DrawBombs(window);
	}
	else
	{
		DrawBombs(window);
		DrawPlayer(window);
	}
}

void Game::DrawPause(RenderWindow& window)
{
	window.draw(pauseRect);
	window.draw(pauseSprite);
	window.draw(continueText);
	window.draw(menuText);
	window.draw(exitText);
}

void Game::DrawEndGame(RenderWindow& window)
{
	window.draw(pauseRect);
	window.draw(pauseSprite);
	window.draw(continueText);
	window.draw(menuText);
	window.draw(exitText);
}

void Game::DrawWindow(RenderWindow& window)
{
	window.setView(view);
	if (gameState == MAIN_MENU)
	{
		menu.Draw(window, gameState, event);
	}
	else
	{
		DrawBackground(window);
		DrawMap(window);
		DrawPlayersHealth(window);
		DrawBombCount(window);
		DrawChest(window);
		SetCorrectDrawOrder(window);
		UpdateBullets(window);
		DrawEnemies(window);
		if (gameState == PAUSE)
		{
			SetPause(window);
			DrawPause(window);
		}
		else if (gameState == END_GAME || gameState == FINISHED)
		{
			DrawEndGame(window);
		}
	}
}

