
#include "BioEnemyShip.h"


BioEnemyShip::BioEnemyShip()
{
	SetSpeed(150);
	SetMaxHitPoints(1);
	SetCollisionRadius(20);
}


void BioEnemyShip::Update(const GameTime& gameTime)
{
	if (IsActive())
	{
		float x = sin(gameTime.GetTotalTime() * Math::PI + GetIndex());
		x *= GetSpeed() * gameTime.GetTimeElapsed() * 1.4f;
		TranslatePosition(x, GetSpeed() * gameTime.GetTimeElapsed());

		if (!IsOnScreen()) Deactivate();
	}

	EnemyShip::Update(gameTime);
}


void BioEnemyShip::Draw(SpriteBatch& spriteBatch)
{
	if (IsActive())
	{
		spriteBatch.Draw(m_pTexture, GetPosition(), Color::WHITE, m_pTexture->GetCenter(), Vector2::ONE, Math::PI, 1);
	}
}
