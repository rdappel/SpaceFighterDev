
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

#pragma once

namespace KatanaEngine
{
	/** @brief Defines the states for text alignment. */
	enum class TextAlign
	{
		LEFT,			/**< Align the text to the left. */
		CENTER,			/**< Align the text to the center. */
		RIGHT			/**< Align the text to the right. */
	};

	/** @brief Defines the methods for sorting sprites before rendering.
		@see SpriteBatch::Begin()
		@see SpriteBatch::Draw() */
	enum class SpriteSortMode
	{
		BACK_TO_FRONT,	/**< Sprites rendered with a lower draw order will render behind those with a higher draw order. */
		DEFERRED,		/**< Sprites rendered first will appear behind those rendered later. */
		FRONT_TO_BACK,	/**< Sprites rendered with a higher draw order will render behind those with a lower draw order. */
		IMMEDIATE,		/**< Sprites will not be batched, and will draw immediately. */
		TEXTURE			/**< Sprites that share a texture will all be rendered together. */
	};

	/** @brief Defines the way in which textures blending will be calculated. */
	enum class BlendState
	{
		ALPHA,				/**< Textures will be blended using premultiplied alpha blending. */
		NON_PREMULTIPLIED,	/**< Textures will be blended using non-premultiplied alpha blending. */
		ADDITIVE,			/**< Textures will be blended using additive blending. */
		SCREEN,				/**< Textures will be blended using screen blending. */
		MULTIPLY,			/**< Textures will be blended using multiply blending. */
		SUBTRACT,			/**< Textures will be blended using subtractive blending. */
		REVERSE_SUBTRACT,	/**< Textures will be blended using reverse subtractive blending. */
		OVERWRITE,			/**< Textures will be drawn without blending. */
		NUM_BLEND_STATES
	};

	/** @brief Enables a group of sprites to be drawn using the same settings. */
	class SpriteBatch
	{

	public:

		SpriteBatch(const uint32_t defaultCount = 1000);
		~SpriteBatch() { }

		/** @brief Begins a sprite batch operation.
			@param sortMode Defines how to sort the sprites for rendering.
			@param blendState Defines how to blend overlaping sprites.
			@param pTransformation Defines a screen space transformation to use. */
		void Begin(const SpriteSortMode sortMode = SpriteSortMode::DEFERRED,
			const BlendState blendState = BlendState::ALPHA,
			ALLEGRO_TRANSFORM *pTransformation = nullptr);

		/** @brief Flushes the sprite batch and restores the device state to how
		it was before Begin was called. */
		void End(/*bool test = false*/);

		/** @brief Adds a sprite to a batch of sprites to be rendered. 
			@param pTexture A pointer to the texture to render.
			@param position The screen position of the sprite.
			@param region The region of the texture to render.
			@param color The color to tint the sprite. The default is Color::White (no tint).
			@param origin The sprite's origin. The default is Vector2::Zero, which is the
			upper left corner of the sprite.
			@param scale The scale factor of the sprite. The default is Vector2::One.
			@param rotation The rotation angle in radians. The default is zero.
			@param drawDepth The depth at which to render the sprite. This is determined
			by the SpriteSortMode arguement that is passed to SpriteBatch::Begin(). The
			default is zero.
			
			@overload */
		void Draw(const Texture *pTexture, const Vector2& position, const Region& region,
			const Color& color = Color::WHITE, const Vector2& origin = Vector2::ZERO,
			const Vector2& scale = Vector2::ONE, const float rotation = 0,
			const float drawDepth = 0);

		/** @brief Adds a sprite to a batch of sprites to be rendered.
			@param pTexture A pointer to the texture to render.
			@param position The screen position of the sprite.
			@param color The color to tint the sprite. The default is Color::White (no tint).
			@param origin The sprite's origin. The default is Vector2::Zero, which is the
			upper left corner of the sprite.
			@param scale The scale factor of the sprite. The default is Vector2::One.
			@param rotation The rotation angle in radians. The default is zero.
			@param drawDepth The depth at which to render the sprite. This is determined
			by the SpriteSortMode arguement that is passed to SpriteBatch::Begin(). The
			default is zero.

			@overload */
		void Draw(const Texture *pTexture, const Vector2& position,
			const Color& color = Color::WHITE, const Vector2& origin = Vector2::ZERO,
			const Vector2& scale = Vector2::ONE, const float rotation = 0,
			const float drawDepth = 0);

		/** @brief Adds a string to a batch of sprites to be rendered.
			@param pFont The font used to draw the text.
			@param pText A pointer to the text to display.
			@param position The screen position of the text.
			@param color The color to tint the text. The default is Color::White (no tint).
			@param alignment The prefered method of aligning the text. The default is
			TextAlign::LEFT.
			@param drawDepth The depth at which to render the sprite. This is determined
			by the SpriteSortMode arguement that is passed to SpriteBatch::Begin(). The
			default is zero.
			@remarks The text pointer should be valid for the duration of the batch.

			@overload */
		//void Draw(const Font* pFont, std::string* pText, const Vector2& position,
		//	const Color& color = Color::WHITE, const TextAlign alignment = TextAlign::LEFT,
		//	const float drawDepth = 0);

		/** @brief Adds a string to a batch of sprites to be rendered.
			@param pFont The font used to draw the text.
			@param text The text to display.
			@param position The screen position of the text.
			@param color The color to tint the text. The default is Color::White (no tint).
			@param alignment The prefered method of aligning the text. The default is
			TextAlign::LEFT.
			@param drawDepth The depth at which to render the sprite. This is determined
			by the SpriteSortMode arguement that is passed to SpriteBatch::Begin(). The
			default is zero.

			@overload */
		void Draw(const Font* pFont, const std::string& text, const Vector2& position,
			const Color& color = Color::WHITE, const TextAlign alignment = TextAlign::LEFT,
			const float drawDepth = 0);

		/** @brief Gets the current settings from the sprite batch.
			@param sortMode Defines how to sort the sprites for rendering.
			@param blendState Defines how to blend overlaping sprites.
			@param pTransformation Defines a screen space transformation to use. */
		void GetBatchSettings(SpriteSortMode &sortMode, BlendState &blendState, ALLEGRO_TRANSFORM *pTransformation);


	private:

		struct Drawable
		{
			bool isBitmap = true;
			ALLEGRO_COLOR color = ALLEGRO_COLOR{};
			int x = 0;
			int y = 0;
			float depth = 0;

			union
			{
				struct // font
				{
					ALLEGRO_FONT* pFont;
					int align;
					unsigned int index;
				};

				struct
				{
					ALLEGRO_BITMAP *pBitmap;
					float rotation;
					int cx;
					int cy;
					int sx;
					int sy;
					int sw;
					int sh;
					float scx;
					float scy;
				};

			} Type = {};

			bool operator<(const Drawable& other) const
			{
				return (depth < other.depth);
			}
		};

		struct CompareBackToFront
		{
			bool operator()(const Drawable* l, const Drawable* r) { return (*l < *r); }
		};

		struct CompareFrontToBack
		{
			bool operator()(const Drawable* l, const Drawable* r) { return (*r < *l); }
		};

		std::vector<Drawable *> m_drawables;
		std::vector<Drawable *> m_inactiveDrawables;
		std::vector<Drawable *>::iterator m_it;

		unsigned short m_lastID = -1;

		SpriteSortMode m_sortMode = SpriteSortMode::DEFERRED;

		BlendState m_blendState = BlendState::ALPHA;

		ALLEGRO_TRANSFORM *m_pTransformation = nullptr;

		bool m_isStarted = false;

		std::vector<std::string> m_textCopies;

		void DrawBitmap(Drawable *drawable);
		void DrawText(Drawable *drawable);

	};
}