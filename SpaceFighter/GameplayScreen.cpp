
#include "GameplayScreen.h"
#include "Level.h"
#include "Level01.h"
#include "Level02.h"

GameplayScreen::GameplayScreen(const int levelIndex, const PlayerOptions& options)
{
	int width = Game::GetScreenWidth();
	int height = Game::GetScreenHeight();

	m_pRenderTarget = new RenderTarget(width, height);

	m_levelIndex = levelIndex;
	m_playerOptions = options;

	SetTransitionInTime(1.0f);
	SetTransitionOutTime(0.5f);

	Show();
}

void GameplayScreen::LoadLevel(const int levelIndex)
{
	if (m_pLevel != nullptr) delete m_pLevel;

	switch (levelIndex)
	{
	case 0: m_pLevel = new Level01(this); break;
	case 1: m_pLevel = new Level02(this); break;
	}

	m_levelIndex = levelIndex;
	m_pLevel->LoadContent(m_pResourceManager);
}

GameplayScreen::~GameplayScreen()
{
	delete m_pLevel;
}

void GameplayScreen::LoadContent(ResourceManager *pResourceManager)
{
	m_pResourceManager = pResourceManager;
	LoadLevel(m_levelIndex);
}

void GameplayScreen::HandleInput(const InputState& input)
{
	m_pLevel->HandleInput(input);
}

void GameplayScreen::Update(const GameTime& gameTime)
{
	m_pLevel->Update(gameTime);
}

void GameplayScreen::Draw(SpriteBatch& spriteBatch)
{
	RenderTarget::Set(m_pRenderTarget);
	//m_pRenderTarget->Clear(Color::BLACK); // fun times!
	spriteBatch.Begin();
	m_pLevel->Draw(spriteBatch);
	spriteBatch.End();

	RenderTarget::Set();
	spriteBatch.Begin();
	spriteBatch.Draw(m_pRenderTarget, Vector2::ZERO);
	spriteBatch.End();
}
