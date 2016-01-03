#include "game.h"

void Game::InitEnemies()
{
	//enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(FLY1_POSITION_X, FLY1_POSITION_Y), FLY_WIDTH, FLY_HEIGHT, "EnemyFly", 1, 1));
	//enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(FLY2_POSITION_X, FLY2_POSITION_Y), FLY_WIDTH, FLY_HEIGHT, "EnemyFly", 1, 1));
	//enemies.push_back(Enemy(mySprites.standAndShootTexture, Vector2f(1400, 200), 38, 43, "EnemyStandAndShoot", 3, 2));
	//enemies.push_back(Enemy(mySprites.standAndShootTexture, Vector2f(1500, 200), 38, 43, "EnemyStandAndShoot", 3, 2));
	//enemies.push_back(Enemy(mySprites.standAndShootTexture, Vector2f(1300, 300), 38, 43, "EnemyStandAndShoot", 3, 2));
	//enemies.push_back(Enemy(mySprites.standAndShootTexture, Vector2f(2100, 300), 38, 43, "EnemyStandAndShoot", 3, 3));
	//enemies.push_back(Enemy(mySprites.standAndShootTexture, Vector2f(2600, 300), 38, 43, "EnemyStandAndShoot", 3, 3));
	//enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(1500, 760), FLY_WIDTH, FLY_HEIGHT, "EnemyFly", 1, 5));
	//enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(1200, 1050), FLY_WIDTH, FLY_HEIGHT, "EnemyFly", 1, 5));
	//enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(2100, 950), FLY_WIDTH, FLY_HEIGHT, "EnemyFly", 1, 6));
	//enemies.push_back(Enemy(mySprites.enemyTexture, Vector2f(1500, 800), FLY_WIDTH, FLY_HEIGHT, "EnemyFly", 1, 5));
	enemies.push_back(Enemy(mySprites.enemyFollowTexture, Vector2f(300, 300), 32, 32, "EnemyFollow", 2, 1));
}

void Game::InitGame()
{

	lastShootPlayer = 0;
	hitTimer = 0;
	room = 1;
	volume = 30;
	//gameState = MAIN_MENU;
	gameState = GAME;
	InitEnemies();
	player = Player(mySprites.heroTexture, Vector2f(PLAYER_POSITION_X, PLAYER_POSITION_Y), PLAYER_WIDTH, PLAYER_HEIGHT, "Hero", 6, mySprites.headTexture);
	view.reset(FloatRect(0, 0, float(WINDOW_WIDTH), float(WINDOW_HEIGHT)));
	myTileMap.LoadMapSprites();
	mySprites.InitImages();
	mySprites.LoadFont();
	mySounds.LoadMusic();
	menu.InitMenu(mySprites.mainMenuTexture, mySprites.font);
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

void Game::DeleteEnemyFromVector()
{
	auto isDead = [](Enemy enemy)
	{
		return (enemy.health < 0);
	};
	enemies.erase(remove_if(enemies.begin(), enemies.end(), isDead), enemies.end());
}

void Game::UpdateEnemies(RenderWindow& window)
{
	DeleteEnemyFromVector();
	for (auto& enemy: enemies)
	{
		if (enemy.alive == true)
		{
			enemy.lastPosition = { enemy.position.x, enemy.position.y };
			enemy.Update(boomb, bullets, time, gameTime, window, room);
			enemy.ExplosionCollision(boomb, gameTime);
			enemy.deathTime = gameTime;
		}
	}
}

void Game::UpdatePlayer()
{
	player.Control(boomb, bullets, time, gameTime, lastShootPlayer);
	player.CheckCollision(myMap, mySprites.wallBackgroundSprite, view, IsRoomCleared());
	player.Moving(time);
	player.CheckEnemyCollidesPlayer(enemies, gameTime, hitTimer, mySounds.playerHurts);
	player.ChangeColorAfterHit(gameTime, hitTimer);
	player.CheckExplosionCollision(boomb, gameTime);
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
			UpdatePlayersBullets(bullet);
			UpdateEnemiesBullets(bullet);
			bullet.CheckCollisionBullet(gameTime, myMap, mySprites.wallBulletSprite, mySounds.tearDestroy);
			bullet.UpdateBullet(time, window, gameTime, mySprites.bulletTexture, mySprites.bulletEffectTexture, mySprites.bulletEffectTextureEnemy);
			bullet.DeleteBullet(gameTime, mySounds.tearDestroy);
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
	time = time / 500;
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
}

void Game::UpdateGame(RenderWindow& window)
{
	UpdateTimePerFrame();
	if (gameState == MAIN_MENU)
	{
		menu.Update(volume, view);
	}
	else if (gameState == GAME)
	{
		mySounds.menuMusic.stop();
		room = InitializeRoom();
		AddChest(view);
		UpdatePlayer();
		UpdateTime();
		UpdateEnemies(window);
		UpdateChests(window);
		UpdateBullets(window);
		UpdateBombs();
		UpdateSounds();
	}	
	ProcessEvents(window);
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
	bombCountSprite.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + 28, view.getCenter().y - WINDOW_HEIGHT / 2 + 32);
	bombCountSprite.setScale(1.5, 1.5);
	window.draw(bombCountSprite);

	Text textBombs;
	textBombs.setFont(mySprites.font);
	textBombs.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + 80, view.getCenter().y - WINDOW_HEIGHT / 2 + 45);
	textBombs.setCharacterSize(20);

	ostringstream toString;
	toString << player.bombCount;
	textBombs.setString(toString.str());

	window.draw(textBombs);
}

void Game::DrawPlayersHealth(RenderWindow& window)
{
	Sprite fullHP;
	fullHP.setTexture(mySprites.heartTexture);
	fullHP.setTextureRect(IntRect(0, 0, 16, 16));
	fullHP.setScale(2, 2);
	Sprite halfHP;
	halfHP.setTexture(mySprites.heartTexture);
	halfHP.setTextureRect(IntRect(16, 0, 16, 16));
	halfHP.setScale(2, 2);
	Sprite emptySprite;
	emptySprite.setTexture(mySprites.heartTexture);
	emptySprite.setTextureRect(IntRect(32, 0, 16, 16));
	emptySprite.setScale(2, 2);

	for (int i = 1; i <= MAX_PLAYER_HEALTH; i++)
	{
		emptySprite.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + i * 35, view.getCenter().y - WINDOW_HEIGHT / 2 + 10);
		window.draw(emptySprite);
	}
	if (player.health > 0)
	{
		for (int i = 1; i <= player.health; i++)
		{
			fullHP.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + i * 35, view.getCenter().y - WINDOW_HEIGHT / 2 + 10);
			window.draw(fullHP);
		}
		if (player.health - int(player.health) != 0)
		{
			halfHP.setPosition(view.getCenter().x - WINDOW_WIDTH / 2 + int(player.health + 1) * 35, view.getCenter().y - WINDOW_HEIGHT / 2 + 10);
			window.draw(halfHP);
		}
	}
}

void Game::DrawEnemies(RenderWindow& window)
{
	for (auto& enemy: enemies)
	{
		if (enemy.alive == true)
		{
			if (enemy.enemyRoom == room)
			{
				window.draw(enemy.sprite);
			}
		}
		else
		{
			enemy.DestroyEffect(gameTime, window, mySprites.poofTexture);
			enemy.position.x = 0;
			enemy.position.y = 0;
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
	myTileMap.drawMap(myMap, window, IsRoomCleared());
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
	if (player.position.y + player.h < boomb.position.y + 64)
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

void Game::DrawWindow(RenderWindow& window)
{
	window.setView(view);
	if (gameState == MAIN_MENU)
	{
		menu.Draw(window, gameState, event);
	}
	else if (gameState == GAME)
	{
		DrawBackground(window);
		DrawMap(window);
		DrawPlayersHealth(window);
		DrawBombCount(window);
		DrawChest(window);
		SetCorrectDrawOrder(window);
		UpdateBullets(window);
		DrawEnemies(window);
	}
}

