#pragma once

#include "KatanaEngine.h"
#include "PlayerOptions.h"

using namespace KatanaEngine;

class OptionsMenuScreen : public MenuScreen
{
	
public:

	OptionsMenuScreen(PlayerOptions& playerOptions);

	virtual ~OptionsMenuScreen() { }

	virtual void LoadContent(ResourceManager *pResourceManager);

	virtual void HandleInput(const InputState& input);

	virtual void Update(const GameTime& gameTime);

	virtual PlayerOptions& GetPlayerOptions() { return m_playerOptions; }


private:

	static const int ITEM_COUNT = 5;

	enum Items { NUM_LIVES, SET_INVINCIBLE, MUSIC_VOL, SOUND_VOL, BACK };
	
	const std::string m_itemTexts[ITEM_COUNT]
		= { "Lives", "Invincible", "Music Volume", "Sound Volume", "Back" };

	PlayerOptions& m_playerOptions;

	float m_inputX = 0;
	bool m_rightRelease = false;
	bool m_leftRelease = false;

};

