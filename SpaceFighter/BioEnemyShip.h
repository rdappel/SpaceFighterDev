
#pragma once

#include "EnemyShip.h"

class BioEnemyShip : public EnemyShip
{

public:

	BioEnemyShip();
	virtual ~BioEnemyShip() { }

	virtual void SetTexture(Texture *pTexture) { m_pTexture = pTexture; }

	virtual void Update(const GameTime& gameTime);

	virtual void Draw(SpriteBatch& spriteBatch);


private:

	Texture *m_pTexture = nullptr;

};