
#pragma once

#include "Ship.h"
#include "PlayerOptions.h"


class PlayerShip : public Ship
{

public:

	PlayerShip(const PlayerOptions& options) : m_options(options) { }
	virtual ~PlayerShip() { }

	virtual void LoadContent(ResourceManager *pResourceManager);

	virtual void Update(const GameTime& gameTime);

	virtual void Draw(SpriteBatch& spriteBatch);

	virtual void HandleInput(const InputState& input);

	virtual void Initialize(Level *pLevel, Vector2 &startPosition);

	virtual Vector2 GetHalfDimensions() const;

	virtual void SetDesiredDirection(const Vector2 direction) { m_desiredDirection.Set(direction); }

	virtual std::string ToString() const { return "Player Ship"; }

	virtual CollisionType GetCollisionType() const { return (CollisionType::PLAYER | CollisionType::SHIP); }

	virtual void ConfineToScreen(const bool isConfined = true) { m_isConfinedToScreen = isConfined; }


protected:

	virtual void SetResponsiveness(const float responsiveness);

	virtual float GetResponsiveness() const { return m_responsiveness; }

	virtual Vector2 GetDesiredDirection() const { return m_desiredDirection; }


private:

	Vector2 m_desiredDirection;
	Vector2 m_velocity;

	float m_responsiveness = 0;

	bool m_isConfinedToScreen = false;

	Texture *m_pTexture = nullptr;

	const PlayerOptions& m_options;

};