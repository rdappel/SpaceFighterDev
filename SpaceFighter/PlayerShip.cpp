
#include "PlayerShip.h"


void PlayerShip::LoadContent(ResourceManager* pResourceManager)
{
	ConfineToScreen();
	SetResponsiveness(0.1);

	m_pTexture = pResourceManager->Load<Texture>("Textures\\PlayerShip.png");

	AudioSample* pAudio = pResourceManager->Load<AudioSample>("Audio\\Effects\\Laser.wav");
	pAudio->SetVolume(m_options.SoundVolume);
	GetWeapon(0)->SetFireSound(pAudio);

	SetPosition(Game::GetScreenCenter() + Vector2::UNIT_Y * 300);
}


void PlayerShip::Update(const GameTime& gameTime)
{
	// Get the velocity for the direction that the player is trying to go.
	Vector2 targetVelocity = m_desiredDirection * GetSpeed() * gameTime.GetTimeElapsed();
	// We can't go from 0-100 mph instantly! This line interpolates the velocity for us.
	m_velocity = Vector2::Lerp(m_velocity, targetVelocity, GetResponsiveness());
	// Move that direction
	TranslatePosition(m_velocity);

	if (m_isConfinedToScreen)
	{
		const int PADDING = 4; // keep the ship 4 pixels from the edge of the screen
		const int TOP = PADDING;
		const int LEFT = PADDING;
		const int RIGHT = Game::GetScreenWidth() - PADDING;
		const int BOTTOM = Game::GetScreenHeight() - PADDING;

		Vector2* pPosition = &GetPosition(); // current position (middle of the ship)
		if (pPosition->X - GetHalfDimensions().X < LEFT) // are we past the left edge?
		{
			// move the ship to the left edge of the screen (keep Y the same)
			SetPosition(LEFT + GetHalfDimensions().X, pPosition->Y);
			m_velocity.X = 0; // remove any velocity that could potentially
							  // keep the ship pinned against the edge
		}
		if (pPosition->X + GetHalfDimensions().X > RIGHT) // right edge?
		{
			SetPosition(RIGHT - GetHalfDimensions().X, pPosition->Y);
			m_velocity.X = 0;
		}
		if (pPosition->Y - GetHalfDimensions().Y < TOP) // top edge?
		{
			SetPosition(pPosition->X, TOP + GetHalfDimensions().Y);
			m_velocity.Y = 0;
		}
		if (pPosition->Y + GetHalfDimensions().Y > BOTTOM) // bottom edge?
		{
			SetPosition(pPosition->X, BOTTOM - GetHalfDimensions().Y);
			m_velocity.Y = 0;
		}
	}

	// do any updates that a normal ship would do.
	// (fire weapons, collide with objects, etc.)
	Ship::Update(gameTime);
}

void PlayerShip::Draw(SpriteBatch& spriteBatch)
{
	if (IsActive())
	{
		spriteBatch.Draw(m_pTexture, GetPosition(), Color::WHITE, m_pTexture->GetCenter());
	}
}

void PlayerShip::HandleInput(const InputState& input)
{
	if (IsActive())
	{
		Vector2 direction = Vector2::ZERO;
		if (input.IsKeyDown(Key::DOWN)) direction.Y++;
		if (input.IsKeyDown(Key::UP)) direction.Y--;
		if (input.IsKeyDown(Key::RIGHT)) direction.X++;
		if (input.IsKeyDown(Key::LEFT)) direction.X--;

		// Normalize the direction
		if (direction.X != 0 && direction.Y != 0)
		{
			direction *= Math::NORMALIZE_PI_OVER4;
		}

		//TriggerType type = TriggerType::NONE;
		//if (input.IsKeyDown(Key::F)) type |= TriggerType::PRIMARY;
		//if (input.IsKeyDown(Key::D)) type |= TriggerType::SECONDARY;
		//if (input.IsKeyDown(Key::S)) type |= TriggerType::SPECIAL;

		GamePadState state = input.GetGamePadState(0);
		if (state.IsConnected)
		{
			// gamepad overrides keyboard input
			Vector2 thumbstick = state.Thumbsticks.Left;
			if (thumbstick.LengthSquared() < 0.08f) thumbstick = Vector2::ZERO;
			direction = thumbstick;

			//type = TriggerType::NONE;
			//if (state.Triggers.Right > 0.5f) type |= TriggerType::PRIMARY;
			//if (state.Triggers.Left > 0.5f) type |= TriggerType::SECONDARY;
			//if (state.IsButtonDown(Button::Y)) type |= TriggerType::SPECIAL;
		}


		SetDesiredDirection(direction);
		//if (type != TriggerType::NONE) FireWeapons(type);

		if (input.IsKeyDown(Key::SPACE)) FireWeapons(TriggerType::PRIMARY);
	}
}


void PlayerShip::Initialize(Level* pLevel, Vector2& startPosition)
{
	SetPosition(startPosition);
}


Vector2 PlayerShip::GetHalfDimensions() const
{
	return m_pTexture->GetCenter();
}

void PlayerShip::SetResponsiveness(const float responsiveness)
{
	m_responsiveness = Math::Clamp(0, 1, responsiveness);
}
