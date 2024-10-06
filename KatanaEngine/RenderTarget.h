
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
	/** @brief Contains a 2D texture that can be used as a render target. */
	class RenderTarget : public Texture
	{

	public:

		/** @brief Instantiate a render target object.
			@param width The width of the render target.
			@param height The height of the render target. */
		RenderTarget(const int width, const int height);
		virtual ~RenderTarget() { }


		/** @brief Set the desired target to render to when drawing.
			@param pTarget A pointer to the desired render target. */
		static void Set(RenderTarget *pTarget = nullptr);

		/** @brief Set the display of the target.
			@param pDisplay A pointer to the display. */
		static void SetDisplay(ALLEGRO_DISPLAY *pDisplay) { s_pDisplay = pDisplay; }

		/** @brief Render targets don't need to be loaded so this simply returns true.
			@param path The path to the desired resource.
			@param pManager A pointer to the ResourceManager that will manage the resource.
			@return Returns true. */
		virtual bool Load(const std::string &path, ResourceManager *pManager) { return true; }

		/** @brief Set the clear color of the render target.
			@param color The desired clear color. */
		virtual void SetClearColor(const Color& color) { m_clearColor = color; }

		/** @brief Reset all of the pixels in the render target to the clear color.
			@param clearColor The desired clear color. */
		virtual void Clear(const Color& clearColor);

		/** @brief Reset all of the pixels in the render target to the clear color. */
		virtual void Clear() { Clear(m_clearColor); }

	private:

		static ALLEGRO_DISPLAY *s_pDisplay;

		Color m_clearColor = Color::TRANSPARENT;

	};
}