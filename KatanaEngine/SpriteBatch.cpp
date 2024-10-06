
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

	SpriteBatch::SpriteBatch(const uint32_t defaultCount)
	{
		for (uint32_t i = 0; i < defaultCount; ++i)
		{
			m_inactiveDrawables.push_back(new Drawable());
		}
	}

	void SpriteBatch::Begin(const SpriteSortMode sortMode,
		const BlendState blendState, ALLEGRO_TRANSFORM* pTransformation)
	{
		m_isStarted = true;
		m_it = m_inactiveDrawables.begin();

		m_sortMode = sortMode;

		if (sortMode == SpriteSortMode::IMMEDIATE)
		{
			if (pTransformation != nullptr) al_use_transform(pTransformation);
		}
		else
		{
			m_pTransformation = pTransformation;
		}

		m_blendState = blendState;

		switch (blendState)
		{
		case BlendState::ALPHA:
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
			break;
		case BlendState::NON_PREMULTIPLIED:
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
			break;
		case BlendState::ADDITIVE:
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
			break;
		case BlendState::SCREEN:
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_SRC_COLOR);
			break;
		case BlendState::SUBTRACT:
			al_set_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ONE, ALLEGRO_ONE);
			break;
		case BlendState::REVERSE_SUBTRACT:
			al_set_blender(ALLEGRO_SRC_MINUS_DEST, ALLEGRO_ONE, ALLEGRO_ONE);
			break;
		case BlendState::MULTIPLY:
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE);
			break;
		case BlendState::OVERWRITE:
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
			break;
		};
	}

	void SpriteBatch::End(/*bool test*/)
	{
		//double time = al_get_time();


		if (m_sortMode != SpriteSortMode::IMMEDIATE)
		{
			if (m_pTransformation != nullptr) al_use_transform(m_pTransformation);

			switch (m_sortMode)
			{
			case SpriteSortMode::BACK_TO_FRONT:
				std::sort(m_drawables.begin(), m_drawables.end(), CompareBackToFront());
				break;

			case SpriteSortMode::FRONT_TO_BACK:
				std::sort(m_drawables.begin(), m_drawables.end(), CompareFrontToBack());
				break;

			case SpriteSortMode::TEXTURE:
				al_hold_bitmap_drawing(true);
				break;
			};
			//if (test) std::cout << "Sort time: " << (float)(al_get_time() - time) * 1000 << ", ";
			//time = al_get_time();

			m_it = m_drawables.begin();
			for (; m_it != m_drawables.end(); ++m_it)
			{
				if ((*m_it)->isBitmap) DrawBitmap(*m_it);
				else DrawText(*m_it);
			}

			//if (test) std::cout << "Draw time: " << (float)(al_get_time() - time) * 1000 << ", ";
			//time = al_get_time();
		}

		m_textCopies.clear();
		m_drawables.clear();

		//if (test) std::cout << "Clear time: " << (float)(al_get_time() - time) * 1000 << std::endl;


		ALLEGRO_TRANSFORM identity;
		al_identity_transform(&identity);
		al_use_transform(&identity);

		m_isStarted = false;
	}

	void SpriteBatch::DrawBitmap(Drawable* pDrawable)
	{
		al_draw_tinted_scaled_rotated_bitmap_region(
			pDrawable->Type.pBitmap,
			pDrawable->Type.sx, pDrawable->Type.sy,
			pDrawable->Type.sw, pDrawable->Type.sh,
			pDrawable->color,
			pDrawable->Type.cx, pDrawable->Type.cy,
			pDrawable->x, pDrawable->y,
			pDrawable->Type.scx, pDrawable->Type.scy,
			pDrawable->Type.rotation, 0);
	}

	void SpriteBatch::DrawText(Drawable* pDrawable)
	{
		const char* text = m_textCopies[pDrawable->Type.index].c_str();

		al_draw_text(
			pDrawable->Type.pFont,
			pDrawable->color,
			pDrawable->x,
			pDrawable->y,
			pDrawable->Type.align,
			text);
	}

	void SpriteBatch::Draw(const Texture* pTexture, const Vector2& position, const Color& color,
		const Vector2& origin, const Vector2& scale, const float rotation, const float drawDepth)
	{
		Region source(0, 0, pTexture->GetWidth(), pTexture->GetHeight());
		Draw(pTexture, position, source, color, origin, scale, rotation, drawDepth);
	}

	void SpriteBatch::Draw(const Texture* pTexture, const Vector2& position, const Region& source,
		const Color& color, const Vector2& origin, const Vector2& scale, const float rotation, const float drawDepth)
	{
		assert(m_isStarted && "You must call Begin() before drawing.");

		Drawable* pDrawable;

		if (m_it != m_inactiveDrawables.end())
		{
			pDrawable = *m_it;
			m_it++;
		}
		else
		{
			pDrawable = new Drawable();
			m_inactiveDrawables.push_back(pDrawable);
			m_it = m_inactiveDrawables.end();
		}

		pDrawable->isBitmap = true;
		pDrawable->Type.pBitmap = pTexture->GetAllegroBitmap();
		pDrawable->Type.cx = origin.X;
		pDrawable->Type.cy = origin.Y;
		pDrawable->Type.scx = scale.X;
		pDrawable->Type.scy = scale.Y;
		pDrawable->Type.rotation = rotation;
		pDrawable->color = color.GetAllegroColor();
		pDrawable->x = position.X;
		pDrawable->y = position.Y;
		pDrawable->depth = drawDepth;

		pDrawable->Type.sx = (&source)->X;
		pDrawable->Type.sy = (&source)->Y;
		pDrawable->Type.sw = (&source)->Width;
		pDrawable->Type.sh = (&source)->Height;

		if (m_sortMode == SpriteSortMode::IMMEDIATE) DrawBitmap(pDrawable);
		else m_drawables.push_back(pDrawable);
	}

	void SpriteBatch::Draw(const Font* pFont, const std::string& text, const Vector2& position,
		const Color& color, const TextAlign alignment, const float drawDepth)
	{
		assert(m_isStarted && "You must call Begin() before drawing.");

		Drawable* pDrawable;

		if (m_it != m_inactiveDrawables.end())
		{
			pDrawable = *m_it;
			m_it++;
		}
		else
		{
			pDrawable = new Drawable();
			m_inactiveDrawables.push_back(pDrawable);
			m_it = m_inactiveDrawables.end();
		}

		unsigned int index = (int)m_textCopies.size();
		m_textCopies.push_back(text);

		pDrawable->isBitmap = false;
		pDrawable->Type.pFont = pFont->GetAllegroFont();
		pDrawable->Type.index = index;
		pDrawable->Type.align = (int)alignment;
		pDrawable->x = position.X;
		pDrawable->y = position.Y;
		pDrawable->color = color.GetAllegroColor();

		if (m_sortMode == SpriteSortMode::IMMEDIATE) DrawText(pDrawable);
		else m_drawables.push_back(pDrawable);

	}


	//void SpriteBatch::Draw(const Font* pFont, std::string* pText, const Vector2& position,
	//	const Color& color, const float drawDepth)
	//{
	//	assert(m_isStarted && "You must call Begin() before drawing.");

	//	Drawable* pDrawable;

	//	if (m_it != m_inactiveDrawables.end())
	//	{
	//		pDrawable = *m_it;
	//		m_it++;
	//	}
	//	else
	//	{
	//		pDrawable = new Drawable();
	//		m_inactiveDrawables.push_back(pDrawable);
	//		m_it = m_inactiveDrawables.end();
	//	}

	//	pDrawable->isBitmap = false;
	//	pDrawable->Type.isTextStored = false;
	//	pDrawable->Type.pFont = pFont->GetAllegroFont();
	//	pDrawable->Type.TextSource.pText = pText;
	//	pDrawable->x = position.X;
	//	pDrawable->y = position.Y;
	//	pDrawable->color = color.GetAllegroColor();

	//	if (m_sortMode == SpriteSortMode::IMMEDIATE) DrawText(pDrawable);
	//	else m_drawables.push_back(pDrawable);
	//}


	void SpriteBatch::GetBatchSettings(SpriteSortMode& sortMode,
		BlendState& blendState, ALLEGRO_TRANSFORM* pTransformation)
	{
		assert(m_isStarted && "Begin must be called before the settings can be retrieved.");

		sortMode = m_sortMode;
		blendState = m_blendState;
		pTransformation = m_pTransformation;
	}
}