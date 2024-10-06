
#pragma once

#include "KatanaEngine.h"
#include "PlayerOptions.h"

using namespace KatanaEngine;

class Level;

class GameplayScreen : public Screen
{

public:

	GameplayScreen(const int levelIndex, const PlayerOptions& options);

	virtual ~GameplayScreen();

	virtual void LoadContent(ResourceManager *pResourceManager);

	virtual void HandleInput(const InputState& input);

	virtual void Update(const GameTime& gameTime);

	virtual void Draw(SpriteBatch& spriteBatch);

	virtual void LoadLevel(const int levelIndex);

	virtual const PlayerOptions& GetPlayerOptions() { return m_playerOptions; }


private:

	int m_levelIndex = 0;

	Level *m_pLevel = nullptr;

	ResourceManager *m_pResourceManager = nullptr;

	PlayerOptions m_playerOptions = PlayerOptions();

};
