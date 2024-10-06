
#include "OptionsMenuScreen.h"


// Callback Function
void OnBackSelect(MenuScreen* pScreen) { pScreen->Exit(); }



OptionsMenuScreen::OptionsMenuScreen(PlayerOptions& playerOptions)
	: m_playerOptions(playerOptions)
{
	SetTransitionInTime(1.0f);
	SetTransitionOutTime(0.5f);

	Show();
}

void OptionsMenuScreen::LoadContent(ResourceManager* pResourceManager)
{

	// Create the menu items
	MenuItem* pItem;
	Font::SetLoadSize(20, true);
	Font* pFont = pResourceManager->Load<Font>("Fonts\\arial.ttf");

	SetDisplayCount(ITEM_COUNT);

	for (int i = 0; i < ITEM_COUNT; i++)
	{
		pItem = new MenuItem(m_itemTexts[i]);
		pItem->SetPosition(Vector2(100, 100 + 50 * i));
		pItem->SetFont(pFont);
		pItem->SetColor(Color::BLUE);
		pItem->SetSelected(i == 0);
		AddMenuItem(pItem);
	}

	GetMenuItem(BACK)->SetSelectCallback(OnBackSelect);
}

void OptionsMenuScreen::HandleInput(const InputState& input)
{
	if (input.IsNewKeyPress(Key::ESCAPE))
	{
		Exit();
		return;
	}

	m_inputX = 0;
	if (input.IsKeyDown(Key::LEFT)) m_inputX -= 1;
	if (input.IsKeyDown(Key::RIGHT)) m_inputX += 1;

	m_leftRelease = input.IsNewKeyRelease(Key::LEFT);
	m_rightRelease = input.IsNewKeyRelease(Key::RIGHT);

	MenuScreen::HandleInput(input);
}

void OptionsMenuScreen::Update(const GameTime& gameTime)
{
	MenuItem* pItem;
	const float OFFSET = sinf(gameTime.GetTotalTime() * 8) * 10 + 10;
	const float VOLUME_CHANGE = 0.003f;

	// Set the menu item colors
	for (int i = 0; i < GetDisplayCount(); i++)
	{
		pItem = GetMenuItem(i);
		pItem->SetAlpha(GetAlpha());

		if (!pItem->IsSelected())
		{
			pItem->SetColor(Color::BLUE);
			pItem->SetTextOffset(Vector2::ZERO);
			pItem->SetText(m_itemTexts[i]);
			continue;
		}
	
		pItem->SetColor(Color::WHITE);
		pItem->SetTextOffset(Vector2::UNIT_X * OFFSET);

		if (i == NUM_LIVES)
		{
			int lives = m_playerOptions.NumberOfLives;
			if (m_leftRelease) lives--;
			if (m_rightRelease) lives++;
			m_playerOptions.NumberOfLives = (int)Math::Clamp(1, 10, lives);

			std::string text = "Lives: " + std::to_string(lives);
			pItem->SetText(text);
		}
		else if (i == SET_INVINCIBLE)
		{
			bool invincible = m_playerOptions.InInvincibleMode;
			if (m_leftRelease || m_rightRelease) invincible = !invincible;
			m_playerOptions.InInvincibleMode = invincible;
			std::string text = "Invincible: " + std::string(invincible ? "On" : "Off");
			pItem->SetText(text);
		}
		else if (i == MUSIC_VOL)
		{
			float volume = m_playerOptions.MusicVolume;
			volume += m_inputX * VOLUME_CHANGE;
			volume = Math::Clamp(0, 1, volume);
			m_playerOptions.MusicVolume = volume;

			std::string text = "Music Volume: " + std::to_string((int)(volume * 100)) + "%";
			pItem->SetText(text);
		}
		else if (i == SOUND_VOL)
		{
			float volume = m_playerOptions.SoundVolume;
			volume += m_inputX * VOLUME_CHANGE;
			volume = Math::Clamp(0, 1, volume);
			m_playerOptions.SoundVolume = volume;

			std::string text = "Sound Volume: " + std::to_string((int)(volume * 100)) + "%";
			pItem->SetText(text);
		}
	}

	MenuScreen::Update(gameTime);
}