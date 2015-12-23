#include "bullet.h"
#include "map.h"

void Bullet::CheckCollisionBullet(float& gameTime, vector<Map>& myMap, Sprite& wallSprite)
{
	if (life == true)
	{
		for (vector<Map>::iterator it = myMap.begin(); it != myMap.end(); it++)
		{
			if (Collision::PixelPerfectTest(bulletSprite, it->sprite))
			{
				deathTime = gameTime;
				life = false;
			}
		}
		if (Collision::PixelPerfectTest(bulletSprite, wallSprite))
		{
			deathTime = gameTime;
			life = false;
		}
	}
}

void Bullet::DeleteBullet(float gameTime)
{
	if (life == true)
	{
		if (gameTime > timeShot + BULLET_LIFE_TIME)
		{
			deathTime = gameTime;
			life = false;
		}

	}
}

void Bullet::BulletDestroyEffect(float gameTime, RenderWindow& window)
{
	if (isPlayers == true)
	{
		if (gameTime < deathTime + BULLET_ANIMATION_STEP_TIME)
		{
			bulletEffectSprite.setTextureRect(IntRect(0, 64, 64, 64));
			bulletEffectSprite.setPosition(x - 16, y - 16);
			window.draw(bulletEffectSprite);
		}
		else if (gameTime > deathTime + BULLET_ANIMATION_STEP_TIME && gameTime < deathTime + BULLET_ANIMATION_STEP_TIME * 2)
		{
			bulletEffectSprite.setTextureRect(IntRect(0, 128, 64, 64));
			bulletEffectSprite.setPosition(x - 16, y - 16);
			window.draw(bulletEffectSprite);
		}
	}
	else
	{
		if (gameTime < deathTime + BULLET_ANIMATION_STEP_TIME)
		{
			bulletEnemyEffectSprite.setTextureRect(IntRect(192, 0, 64, 64));
			bulletEnemyEffectSprite.setPosition(x - 16, y - 16);
			window.draw(bulletEnemyEffectSprite);
		}
		else if (gameTime > deathTime + BULLET_ANIMATION_STEP_TIME && gameTime < deathTime + BULLET_ANIMATION_STEP_TIME * 2)
		{
			bulletEnemyEffectSprite.setTextureRect(IntRect(64, 64, 64, 64));
			bulletEnemyEffectSprite.setPosition(x - 16, y - 16);
			window.draw(bulletEnemyEffectSprite);
		}
	}
}

void Bullet::UpdateBullet(float time, RenderWindow & window, float gameTime, Texture & bulletTexture, Texture& bulletEffectTexture, Texture& bulletEnemyEffectTexture, vector<Map>& myMap, Sprite& wallSprite)
{
	if (life == true)
	{
		bulletSprite.setTexture(bulletTexture);
		bulletEffectSprite.setTexture(bulletEffectTexture);
		bulletEnemyEffectSprite.setTexture(bulletEnemyEffectTexture);
		if (isPlayers == true)
		{
			bulletSprite.setTextureRect(IntRect(96, 96, BULLET_SIDE, BULLET_SIDE));
		}
		else
		{
			bulletSprite.setTextureRect(IntRect(96, 32, BULLET_SIDE, BULLET_SIDE));
		}

		switch (direction)
		{
		case 0: dx = float(-speed*0.66); dy = float(-speed*0.66); break;
		case 1: dx = float(-speed*0.66); dy = float(speed*0.66); break;
		case 2: dx = float(speed*0.66); dy = float(-speed*0.66); break;
		case 3: dx = float(speed*0.66); dy = float(speed*0.66); break;
		case 4: dx = -speed; dy = 0; break;
		case 5: dx = 0; dy = -speed; break;
		case 6: dx = 0; dy = speed; break;
		case 7: dx = speed; dy = 0; break;
		case 8: dx = 0; dy = speed; break;
		}

		x += dx * time;
		y += dy * time;

		bulletSprite.setPosition(x, y);

		DeleteBullet(gameTime);
		CheckCollisionBullet(gameTime, myMap, wallSprite);

		window.draw(bulletSprite);
	}
}