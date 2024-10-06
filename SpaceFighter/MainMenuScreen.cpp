
#include <string>
#include "MainMenuScreen.h"
#include "OptionsMenuScreen.h"
#include "GameplayScreen.h"

// Callback Functions
void OnStartGameSelect(MenuScreen *pScreen)
{
	MainMenuScreen* pMainMenuScreen = (MainMenuScreen*)pScreen;
	PlayerOptions& playerOptions = pMainMenuScreen->GetPlayerOptions();
	GameplayScreen *pGameplayScreen = new GameplayScreen(0, playerOptions);
	pScreen->GetScreenManager()->AddScreen(pGameplayScreen);
}

void OnOptionsSelect(MenuScreen *pScreen)
{
	MainMenuScreen* pMainMenuScreen = (MainMenuScreen*)pScreen;
	PlayerOptions& playerOptions = pMainMenuScreen->GetPlayerOptions();
	OptionsMenuScreen *pOptionsMenuScreen = new OptionsMenuScreen(playerOptions);
	pScreen->GetScreenManager()->AddScreen(pOptionsMenuScreen);
}

void OnQuitSelect(MenuScreen *pScreen)
{
	MainMenuScreen *pMainMenuScreen = (MainMenuScreen *)pScreen;
	pMainMenuScreen->SetQuitFlag();
	pMainMenuScreen->Exit();
}

void OnScreenRemove(Screen *pScreen)
{
	MainMenuScreen *pMainMenuScreen = (MainMenuScreen *)pScreen;
	if (pMainMenuScreen->IsQuittingGame()) pScreen->GetGame()->Quit();
}



MainMenuScreen::MainMenuScreen()
{
	m_pTexture = nullptr;

	SetRemoveCallback(OnScreenRemove);

	SetTransitionInTime(1.0f);
	SetTransitionOutTime(0.5f);

	Show();
}

void MainMenuScreen::LoadContent(ResourceManager *pResourceManager)
{
	// Logo
	m_pTexture = pResourceManager->Load<Texture>("Textures\\Logo.png");
	m_texturePosition = Game::GetScreenCenter() - Vector2::UNIT_Y * 150;

	// Create the menu items
	const int COUNT = 3;
	MenuItem *pItem;
	Font::SetLoadSize(20, true);
	Font *pFont = pResourceManager->Load<Font>("Fonts\\arial.ttf");

	SetDisplayCount(COUNT);

	enum Items { START_GAME, OPTIONS, QUIT };
	std::string text[COUNT] = { "Start Game", "Options", "Quit" };

	for (int i = 0; i < COUNT; i++)
	{
		pItem = new MenuItem(text[i]);
		pItem->SetPosition(Vector2(100, 100 + 50 * i));
		pItem->SetFont(pFont);
		pItem->SetColor(Color::BLUE);
		pItem->SetSelected(i == 0);
		AddMenuItem(pItem);
	}

	GetMenuItem(START_GAME)->SetSelectCallback(OnStartGameSelect);
	GetMenuItem(OPTIONS)->SetSelectCallback(OnOptionsSelect);
	GetMenuItem(QUIT)->SetSelectCallback(OnQuitSelect);
}

void MainMenuScreen::Update(const GameTime& gameTime)
{
	MenuItem *pItem;

	const float OFFSET = sinf(gameTime.GetTotalTime() * 8) * 10 + 10;

	// Set the menu item colors
	for (int i = 0; i < GetDisplayCount(); i++)
	{
		pItem = GetMenuItem(i);
		pItem->SetAlpha(GetAlpha());

		if (pItem->IsSelected())
		{
			pItem->SetColor(Color::WHITE);
			//Vector2 textOffset = pItem->GetTextOffset();
			pItem->SetTextOffset(Vector2::UNIT_X * OFFSET);
		}
		else
		{
			pItem->SetColor(Color::BLUE);
			pItem->SetTextOffset(Vector2::ZERO);
		}
	}

	MenuScreen::Update(gameTime);
}

void MainMenuScreen::Draw(SpriteBatch& spriteBatch)
{
	spriteBatch.Begin();
	spriteBatch.Draw(m_pTexture, m_texturePosition, Color::WHITE * GetAlpha(), m_pTexture->GetCenter());
	spriteBatch.End();

	MenuScreen::Draw(spriteBatch);
}
