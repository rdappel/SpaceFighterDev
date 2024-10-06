
/*
     ██╗  ██╗  █████╗  ████████╗  █████╗  ███╗   ██╗  █████╗ 
	 ██║ ██╔╝ ██╔══██╗ ╚══██╔══╝ ██╔══██╗ ████╗  ██║ ██╔══██╗
	 █████╔╝  ███████║    ██║    ███████║ ██╔██╗ ██║ ███████║
	 ██╔═██╗  ██╔══██║    ██║    ██╔══██║ ██║╚██╗██║ ██╔══██║
	 ██║  ██╗ ██║  ██║    ██║    ██║  ██║ ██║ ╚████║ ██║  ██║
	 ╚═╝  ╚═╝ ╚═╝  ╚═╝/\  ╚═╝    ╚═╝  ╚═╝ ╚═╝  ╚═══╝ ╚═╝  ╚═╝
   /vvvvvvvvvvvvvvvvvvv \=========================================,
   `^^^^^^^^^^^^^^^^^^^ /---------------------------------------"
        Katana Engine \/ © 2012 - Shuriken Studios LLC
			    http://www.shurikenstudios.com
*/

#include "KatanaEngine.h"

namespace KatanaEngine
{
	ParticleManager::ParticleManager(Game *pGame)
	{
		m_pGame = pGame;
	}


	ResourceManager *ParticleManager::GetResourceManager() const
	{
		return GetGame()->GetResourceManager();
	}


	void ParticleManager::Update(const GameTime& gameTime)
	{
		m_it = m_particles.begin();
		for (; m_it != m_particles.end();)
		{
			Particle *pParticle = *m_it;

			if (pParticle->IsActive())
			{
				pParticle->Update(gameTime);

				if (pParticle->IsActive()) ++m_it;
				else m_it = m_particles.erase(m_it);
			}
		}
	}


	void ParticleManager::Draw(SpriteBatch& spriteBatch)
	{
		spriteBatch.Begin(SpriteSortMode::TEXTURE, BlendState::ADDITIVE);
		m_it = m_particles.begin();
		for (; m_it != m_particles.end(); ++m_it)
		{
			(*m_it)->Draw(spriteBatch);
		}
		spriteBatch.End();
	}
}