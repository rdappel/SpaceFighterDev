
#include "Level.h"
#include "EnemyShip.h"
#include "Blaster.h"
#include "GameplayScreen.h"


// Collision Callback Functions
void PlayerShootsEnemy(GameObject *pObject1, GameObject *pObject2)
{
	bool m = pObject1->HasMask(CollisionType::ENEMY);
	EnemyShip *pEnemyShip = (EnemyShip *)((m) ? pObject1 : pObject2);
	Projectile *pPlayerProjectile = (Projectile *)((!m) ? pObject1 : pObject2);
	pEnemyShip->Hit(pPlayerProjectile->GetDamage());
	pPlayerProjectile->Deactivate();
}

void PlayerCollidesWithEnemy(GameObject *pObject1, GameObject *pObject2)
{
	bool m = pObject1->HasMask(CollisionType::PLAYER);
	PlayerShip *pPlayerShip = (PlayerShip *)((m) ? pObject1 : pObject2);
	EnemyShip *pEnemyShip = (EnemyShip *)((!m) ? pObject1 : pObject2);
	pPlayerShip->Hit(std::numeric_limits<float>::max());
	pEnemyShip->Hit(std::numeric_limits<float>::max());
}


Level::Level(GameplayScreen* pGameplayScreen)
{
	m_pGameplayScreen = pGameplayScreen;

	m_sectorSize.X = 64;
	m_sectorSize.Y = 64;

	m_sectorCount.X = (float)(Game::GetScreenWidth() / (int)m_sectorSize.X) + 1;
	m_sectorCount.Y = (float)(Game::GetScreenHeight() / (int)m_sectorSize.Y) + 1;

	m_totalSectorCount = m_sectorCount.X * m_sectorCount.Y;

	m_pSectors = new std::vector<GameObject *>[m_totalSectorCount];
	m_pCollisionManager = new CollisionManager();
	
	GameObject::SetCurrentLevel(this);

	// Setup player ship
	const PlayerOptions& options = pGameplayScreen->GetPlayerOptions();
	m_pPlayerShip = new PlayerShip(options);
	Blaster *pBlaster = new Blaster(true);
	pBlaster->SetProjectilePool(&m_projectiles);
	m_pPlayerShip->AttachWeapon(pBlaster, Vector2::UNIT_Y * -20);
	m_pPlayerShip->SetInvulnurable(options.InInvincibleMode);

	for (int i = 0; i < 100; i++)
	{
		Projectile *pProjectile = new Projectile();
		m_projectiles.push_back(pProjectile);
		AddGameObject(pProjectile);
	}
	
	m_pPlayerShip->Activate();
	AddGameObject(m_pPlayerShip);

	// Setup collision types
	CollisionManager *pC = GetCollisionManager();

	CollisionType playerShip = (CollisionType::PLAYER | CollisionType::SHIP);
	CollisionType playerProjectile = (CollisionType::PLAYER | CollisionType::PROJECTILE);
	CollisionType enemyShip = (CollisionType::ENEMY | CollisionType::SHIP);

	pC->AddNonCollisionType(playerShip, playerProjectile);
	pC->AddCollisionType(playerProjectile, enemyShip, PlayerShootsEnemy);
	pC->AddCollisionType(playerShip, enemyShip, PlayerCollidesWithEnemy);
}

Level::~Level()
{
	delete[] m_pSectors;
	delete m_pCollisionManager;
	
	m_gameObjectIt = m_gameObjects.begin();
	for (; m_gameObjectIt != m_gameObjects.end(); m_gameObjectIt++)
	{
		delete (*m_gameObjectIt);
	}
}


void Level::LoadContent(ResourceManager *pResourceManager)
{
	m_pPlayerShip->LoadContent(pResourceManager);
}


void Level::HandleInput(const InputState& input)
{
	m_pPlayerShip->HandleInput(input);
}


void Level::Update(const GameTime& gameTime)
{
	for (unsigned int i = 0; i < m_totalSectorCount; i++)
	{
		m_pSectors[i].clear();
	}

	m_gameObjectIt = m_gameObjects.begin();
	for (; m_gameObjectIt != m_gameObjects.end(); m_gameObjectIt++)
	{
		GameObject *pGameObject = (*m_gameObjectIt);
		pGameObject->Update(gameTime);
	}

	for (unsigned int i = 0; i < m_totalSectorCount; i++)
	{
		if (m_pSectors[i].size() > 1)
		{
			CheckCollisions(m_pSectors[i]);
		}
	}
}


void Level::UpdateSectorPosition(GameObject *pGameObject)
{
	Vector2 position = pGameObject->GetPosition();
	//Vector2 previousPosition = pGameObject->GetPreviousPosition();
	Vector2 halfDimensions = pGameObject->GetHalfDimensions();

	int minX = (int)(position.X - halfDimensions.X - 0.5f);
	int maxX = (int)(position.X + halfDimensions.X + 0.5f);
	int minY = (int)(position.Y - halfDimensions.Y - 0.5f);
	int maxY = (int)(position.Y + halfDimensions.Y + 0.5f);

	//// Expand to contain the previous positions
	minX = (int)Math::Clamp(0, m_sectorCount.X - 1, (float)(minX / (int)m_sectorSize.X));
	maxX = (int)Math::Clamp(0, m_sectorCount.X - 1, (float)(maxX / (int)m_sectorSize.X));
	minY = (int)Math::Clamp(0, m_sectorCount.Y - 1, (float)(minY / (int)m_sectorSize.Y));
	maxY = (int)Math::Clamp(0, m_sectorCount.Y - 1, (float)(maxY / (int)m_sectorSize.Y));


	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			int index = y * (int)m_sectorCount.X + x;

			m_pSectors[index].push_back(pGameObject);
		}
	}
}


void Level::CheckCollisions(std::vector<GameObject *> &gameObjects)
{
	const unsigned int objectCount = (unsigned int)gameObjects.size();

	GameObject *pFirst, *pSecond;

	for (unsigned int i = 0; i < objectCount - 1; i++)
	{
		pFirst = gameObjects[i];
		if (pFirst->IsActive())
		{
			for (unsigned int j = i + 1; j < objectCount; j++)
			{
				if (!pFirst->IsActive()) continue;

				pSecond = gameObjects[j];
				if (pSecond->IsActive())
				{
					m_pCollisionManager->CheckCollision(pFirst, pSecond);
				}
			}
		}
	}
}

void Level::Draw(SpriteBatch& spriteBatch)
{
	spriteBatch.Begin();

	m_gameObjectIt = m_gameObjects.begin();
	for (; m_gameObjectIt != m_gameObjects.end(); m_gameObjectIt++)
	{
		GameObject *pGameObject = (*m_gameObjectIt);
		pGameObject->Draw(spriteBatch);
	}

	spriteBatch.End();
}