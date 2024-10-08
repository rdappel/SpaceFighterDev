﻿
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
	int Game::s_screenWidth = 1600;
	int Game::s_screenHeight = 900;
	int Game::s_previousScreenWidth = 0;
	int Game::s_previousScreenHeight = 0;

	std::string Game::s_contentDirectory = "..\\Content\\";
	std::string Game::s_windowTitle = "Game";

	Game::Game()
	{
		m_isInitialized = al_init();

		al_install_audio();
		al_init_acodec_addon();

		srand(time(nullptr));

		m_requireOpenGL = false;

		m_pInput = nullptr;
		m_pFrameCounterFont = nullptr;

		m_currentTime = al_get_time();
		m_previousTime = al_get_time();
		m_frameCounter = 0;
		m_actualFramesPerSec = 60;
		SetTargetFramesPerSecond(60);

		s_windowTitle = "";

		m_pResourceManager = new ResourceManager();
		m_pScreenManager = nullptr;
		m_pParticleManager = nullptr;

		m_pSpriteBatch = new SpriteBatch();

		AudioSample::ReserveSamples(10);

		srand(time(nullptr));
	}


	Game::~Game()
	{
		m_pResourceManager->UnloadAllResources();

		delete m_pResourceManager;
		delete m_pSpriteBatch;
		delete m_pInput;
		delete m_pGameTime;
	}


	void Game::Update(const GameTime& gameTime)
	{
		if (GetScreenManager())
		{
			GetScreenManager()->HandleInput(*m_pInput);
			GetScreenManager()->Update(gameTime);
		}

		if (GetParticleManager())
		{
			GetParticleManager()->Update(gameTime);
		}
	}

	void Game::Draw(SpriteBatch& spriteBatch)
	{
		if (GetScreenManager())
		{
			GetScreenManager()->Draw(spriteBatch);
		}
	}


	int Game::Run()
	{
		m_isRunning = true;

		if (!m_isInitialized)
		{
			const char message[] = "Failed to initialize the game.";
			al_show_native_message_box(nullptr, nullptr, nullptr, message, nullptr, 0);
			return -1;
		}

		al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_REQUIRE);
		if (m_isFullScreen) al_set_new_display_flags(ALLEGRO_FULLSCREEN);
		if (m_requireOpenGL) al_set_new_display_flags(ALLEGRO_OPENGL);

		m_pDisplay = al_create_display(GetScreenWidth(), GetScreenHeight());
		if (!m_pDisplay)
		{
			const char message[] = "Failed to initialize display.";
			al_show_native_message_box(nullptr, nullptr, nullptr, message, nullptr, 0);
			return -1;
		}


		if (s_windowTitle.length() == 0) s_windowTitle = GetName();
		al_set_window_title(m_pDisplay, s_windowTitle.c_str());
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
		al_inhibit_screensaver(true);

		RenderTarget::SetDisplay(m_pDisplay);
		bool redraw = true;


		// Create and register events
		ALLEGRO_EVENT_QUEUE *pEventQueue = al_create_event_queue();
		ALLEGRO_TIMER *pTimer = al_create_timer(m_inverseTargetFrames);

		al_register_event_source(pEventQueue, al_get_timer_event_source(pTimer));
		al_register_event_source(pEventQueue, al_get_display_event_source(m_pDisplay));

		m_pInput = new InputState();

		ALLEGRO_EVENT_SOURCE *joystickEventSource = al_get_joystick_event_source();
		if (joystickEventSource)
		{
			al_register_event_source(pEventQueue, joystickEventSource);
		}

		ResetGameTime();

		LoadContent(m_pResourceManager);

		al_start_timer(pTimer);
		ALLEGRO_EVENT alEvent;

		while (IsRunning())
		{
			al_wait_for_event(pEventQueue, &alEvent);

			switch (alEvent.type)
			{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				al_stop_timer(pTimer);
				Quit();
				break;

			case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
				m_pInput->UpdateConfigurationEvent();
				break;

			case ALLEGRO_EVENT_JOYSTICK_AXIS:
				m_pInput->UpdateAxisEvent(alEvent);
				break;
			case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
				m_pInput->UpdateButtonEvent(alEvent, ButtonState::RELEASED);
				break;
			case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
				m_pInput->UpdateButtonEvent(alEvent, ButtonState::PRESSED);
				break;

			case ALLEGRO_EVENT_TIMER:
				m_pGameTime->Update();
				Update(*m_pGameTime);
				m_pInput->Update();
				redraw = true;
				break;
			}

			if (redraw && al_event_queue_is_empty(pEventQueue))
			{
				redraw = false;

				al_clear_to_color(al_map_rgb(0, 0, 0));

				Draw(*m_pSpriteBatch);

				m_frameCounter++;

				al_flip_display();
			}
		}

		UnloadContent();

		al_destroy_display(m_pDisplay);

		return 0;
	}

	void Game::DisplayFrameRate()
	{
		m_currentTime = al_get_time();

		if (m_currentTime - m_previousTime >= 1.0)
		{
			m_actualFramesPerSec = m_frameCounter / (m_currentTime - m_previousTime);
			m_frameCounter = 0;
			m_previousTime = m_currentTime;
		}

		if (m_pFrameCounterFont)
		{
			Color color(0, 1, 0);

			float percent = (float)(m_actualFramesPerSec * m_inverseTargetFrames);

			if (percent >= 0.98f)
			{
				color = Color(0, 0.5, 0);
			}
			if (percent < 0.98f && percent >= 0.8f)
			{
				color = Color(1, 1, 0);
			}
			else if (percent < 0.8f && percent >= 0.6f)
			{
				color = Color(1, 0.5, 0);
			}
			else if (percent < 0.6f)
			{
				color = Color(1, 0, 0);
			}

			char buffer[16];
			sprintf_s(buffer, "FPS: %.2f", (float)m_actualFramesPerSec);
			std::string text = buffer;

			m_pSpriteBatch->Begin();
			m_pSpriteBatch->Draw(m_pFrameCounterFont, text, Vector2(10, 10), color);
			m_pSpriteBatch->End();
		}
		else
		{
			std::cout << "FPS: " << m_actualFramesPerSec << std::endl;
		}
	}

	void Game::SetTargetFramesPerSecond(int const frames)
	{
		m_targetFramesPerSecond = frames;
		m_inverseTargetFrames = 1.0 / frames;
	}

	void Game::ToggleFullScreen()
	{
		if (m_pDisplay == nullptr) return;

		if (s_previousScreenHeight == 0 || s_previousScreenWidth == 0)
		{
			s_previousScreenHeight = s_screenHeight;
			s_previousScreenWidth = s_screenWidth;
		}

		int width = s_previousScreenWidth;
		int height = s_previousScreenHeight;

		if (!m_isFullScreen)
		{
			ALLEGRO_MONITOR_INFO info;
			al_get_monitor_info(0, &info);
			width = info.x2 - info.x1;
			height = info.y2 - info.y1;
		}

		bool resized = al_resize_display(m_pDisplay, width, height);
		bool setFlag = al_set_display_flag(m_pDisplay, ALLEGRO_FULLSCREEN_WINDOW, !m_isFullScreen);

		if (resized || setFlag)
		{
			s_previousScreenHeight = s_screenHeight;
			s_previousScreenWidth = s_screenWidth;

			s_screenWidth = width;
			s_screenHeight = height;

			m_isFullScreen = !m_isFullScreen;
			al_acknowledge_resize(m_pDisplay);
		}
	}
}