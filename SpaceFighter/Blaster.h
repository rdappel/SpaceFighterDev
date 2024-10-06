
#pragma once

#include "Weapon.h"

class Blaster : public Weapon
{

public:

	Blaster(const bool isActive);

	virtual ~Blaster() { }

	virtual void Update(const GameTime& gameTime);

	virtual bool CanFire() const { return m_cooldown <= 0; }

	virtual void ResetCooldown() { m_cooldown = 0; }

	virtual float GetCooldownSeconds() { return m_cooldownSeconds; }

	virtual void SetCooldownSeconds(const float seconds) { m_cooldownSeconds = seconds; }

	virtual void Fire(TriggerType triggerType);


private:

	float m_cooldown;
	float m_cooldownSeconds;

};