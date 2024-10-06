
#include "Blaster.h"


Blaster::Blaster(const bool isActive) : Weapon(isActive)
{
	m_cooldown = 0;
	m_cooldownSeconds = 0.35;
}


void Blaster::Update(const GameTime& gameTime)
{
	if (m_cooldown > 0) m_cooldown -= gameTime.GetTimeElapsed();
}


void Blaster::Fire(TriggerType triggerType)
{
	if (IsActive() && CanFire())
	{
		if (triggerType.Contains(GetTriggerType()))
		{
			Projectile* pProjectile = GetProjectile();
			if (pProjectile)
			{
				AudioSample* pAudio = GetFireSound();
				if (pAudio) pAudio->Play();

				pProjectile->Activate(GetPosition(), true);
				m_cooldown = m_cooldownSeconds;
			}
		}
	}
}
