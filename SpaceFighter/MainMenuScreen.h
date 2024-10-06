#pragma once

#include "KatanaEngine.h"
#include "PlayerOptions.h"


using namespace KatanaEngine;

class MainMenuScreen : public MenuScreen
{

public:

	MainMenuScreen();

	virtual ~MainMenuScreen() { }

	virtual void LoadContent(ResourceManager *pResourceManager);
	
	virtual void Update(const GameTime& gameTime);

	virtual void Draw(SpriteBatch& spriteBatch);


	virtual void SetQuitFlag() { m_isQuittingGame = true; }

	virtual bool IsQuittingGame() { return m_isQuittingGame; }

	virtual PlayerOptions& GetPlayerOptions() { return m_playerOptions; }


private:
	
	Texture *m_pTexture;

	Vector2 m_texturePosition;

	bool m_isQuittingGame = false;

	PlayerOptions m_playerOptions;

};