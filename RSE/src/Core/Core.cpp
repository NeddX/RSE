#include "include/Core.h"

namespace Advres::RSE
{
	SDL_Window* RSECore::sdlWindow = nullptr;
	SDL_Renderer* RSECore::sdlRenderer = nullptr;
	SDL_Event RSECore::sdlEvent;
	SDL_GLContext RSECore::glContext;
	std::vector<BoxCollider2D*> RSECore::colliders;
	int RSECore::m_ScreenWidth = 0;
	int RSECore::m_ScreenHeight = 0;
	std::shared_ptr<EntityManager> RSECore::entityManager;
	std::unique_ptr<EntityManager> RSECore::UIManager = std::make_unique<EntityManager>();
	std::chrono::system_clock::time_point RSECore::m_Tp1, RSECore::m_Tp2;
	std::chrono::high_resolution_clock::time_point RSECore::m_StartTp;
	bool RSECore::disableCamera = false;
	std::vector<Rect> RSECore::m_DebugRects;
	std::vector<std::pair<Vector2, Vector2>> RSECore::m_DebugLines;
	const Camera2DComponent* CameraModule::m_MainCamera = nullptr;

	RSECore::RSECore()
	{
		// do stuff
	}

	RSECore::~RSECore()
	{
		//delete entityManager;

		SDL_DestroyWindow(sdlWindow);
		SDL_DestroyRenderer(sdlRenderer);
		SDL_Quit();

		fmt::println("RSE Disposed.");
	}

	RSEStatus RSECore::Init(const char* windowTitle, int width, int height, 
		int fpsLimit, int x, int y, RSEWindowState windowState)
	{
		// Initialize fields
		m_ScreenWidth = width;
		m_ScreenHeight = height;
		m_Fps = 0;
		m_FrameCount = 0;
		m_TargetFps = fpsLimit;
		m_FrameDelay = (m_TargetFps > 0) ? 1000 / m_TargetFps : 0;
		m_WindowTitle = windowTitle;
		m_StartTp = std::chrono::high_resolution_clock::now();
		
		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		
		// Enable sprite batching
		// Pretty sure SDL does this by default but whatever.
		// Also, it talks about inefficent rendering when batching is enabled but so far
		// no issue has occured so im gonna force this.
		SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

		// Initialize SDL, create a window and a renderer for it
		uint32_t flags = 0;
		switch (windowState)
		{
			case RSEWindowState::FULLSCREEN:
				flags = SDL_WINDOW_FULLSCREEN;
				break;
			case RSEWindowState::WINDOWED:
				flags = SDL_WINDOW_SHOWN;
				break;
		}

		if (SDL_Init(SDL_INIT_EVERYTHING) >= 0)
		{
			printf("SDL Initialized\n");
			sdlWindow = SDL_CreateWindow(
				windowTitle,
				x,
				y,
				width,
				height,
				flags | SDL_WINDOW_OPENGL);
			
			if (sdlWindow)
			{
				glContext = SDL_GL_CreateContext(sdlWindow);
				SDL_GL_MakeCurrent(sdlWindow, glContext);
				//glewInit();
				//auto glVersion = glGetString(GL_VERSION);
				//fmt::println("OpenGL Version: {}", glVersion);
				fmt::println("SDL Window created\nSuccessfully created OpenGL context from SDL_Window");
				fmt::println("GLEW Initialized");
				sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
				
				if (sdlRenderer) fmt::println("SDL Renderer initialized");
				else return RSEStatus::RSE_SDL_RENDERER_INIT_FAILURE;
			}
			else return RSEStatus::RSE_SDL_WINDOW_INIT_FAILURE;
		}
		else return RSEStatus::RSE_SDL_INIT_FAILURE;

		// Set window title
		// WIN32 EXCL
		//if (m_WindowTitle) SetConsoleTitle(m_WindowTitle);

		// Disable C++'s stream synchronisation with standard C streams
		// to increase IO speed (presumably)
		std::ios::sync_with_stdio(false);

		// Initialize Input class
		Input::Init();

		// Initialize Entity manager
		//entityManager = new EntityManager();
		entityManager = std::make_shared<EntityManager>();

		// Call Start
		Start();

		// Get the current time for calculating the frame rate
		m_StartTp = std::chrono::high_resolution_clock::now();

		// Set the the engine thread to run and start it
		m_Tp1 = std::chrono::system_clock::now();
		m_Tp2 = std::chrono::system_clock::now();
		m_Running = true;
		EngineThread();

		// Return RSE_OK, everything went well
		return RSEStatus::RSE_OK;
	}

	void RSECore::Start()
	{

	}

	void RSECore::Update(float deltaTime)
	{

	}

	void RSECore::Render(float deltaTime)
	{
		// Render
	}

	bool RSECore::IsInView(Rect rect) noexcept
	{
		if (!camera->Active()) return true;
		return
			rect.x + rect.w >= camera->m_MainCamera->m_Viewport.x &&
			camera->m_MainCamera->m_Viewport.x + camera->m_MainCamera->m_Viewport.w >= rect.x &&
			rect.y + rect.h >= camera->m_MainCamera->m_Viewport.y &&
			camera->m_MainCamera->m_Viewport.y + camera->m_MainCamera->m_Viewport.h >= rect.y;
	}

	void RSECore::EngineThread()
	{
		// The engine loop
		while (m_Running)
		{
			// Handle delta
			m_Tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = m_Tp2 - m_Tp1;
			m_Tp1 = m_Tp2;
			float delta_time = elapsedTime.count();

			// Limit frame rate
			if (m_FrameDelay > delta_time && m_TargetFps > 0) SDL_Delay(m_FrameDelay - (unsigned int) delta_time);

			// Handle events
			EventHandler();

			// Rendering
			SDL_RenderClear(sdlRenderer);

			// Call Render
			//(this->m_RenderMethod)(delta_time);
			Render(delta_time);

			// doesnt matter
			entityManager->Render(delta_time);
			//UIManager->Render(delta_time);
#ifdef _DEBUG
			for (const auto& r : m_DebugRects)
			{
				SDL_SetRenderDrawColor(sdlRenderer, 161, 52, 235, 0);
				SDL_Rect sr = { r.x, r.y, r.w, r.h };
				SDL_RenderDrawRect(sdlRenderer, &sr);
			}
			if (!m_DebugRects.empty()) m_DebugRects.clear();
			for (const auto& l : m_DebugLines)
			{
				SDL_SetRenderDrawColor(sdlRenderer, 161, 52, 235, 0);
				SDL_RenderDrawLine(sdlRenderer, l.first.x, l.first.y, l.second.x, l.second.y);
			}
			if (!m_DebugLines.empty()) m_DebugLines.clear();
#endif
			SDL_SetRenderDrawColor(sdlRenderer, 100, 100, 100, 255);
			SDL_RenderPresent(sdlRenderer);
			
			// Update scenes
			entityManager->Update(delta_time);

			// Call the Update function
			//(this->m_UpdateMethod)(delta_time);
			Update(delta_time);
			
			m_FrameCount++;

			// Calculate the framerate
			m_Fps = (int)(1.0f / delta_time);
		}
	}

	void RSECore::EventHandler()
	{
		SDL_PollEvent(&sdlEvent);
		switch (sdlEvent.type)
		{
			case SDL_QUIT:
				m_Running = false;
				break;
			case SDL_KEYDOWN:
			{
				auto key = sdlEvent.key.keysym.sym;
				if (Input::keys.find(key) != Input::keys.end()) Input::keys[key] = true;
				break;
			}
			case SDL_KEYUP:
			{
				auto key = sdlEvent.key.keysym.sym;
				if (Input::keys.find(key) != Input::keys.end()) Input::keys[key] = false; 
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				Input::mouse.button = (MouseBtn) sdlEvent.button.button;
				break;
			}
			case SDL_MOUSEBUTTONUP:
			{
				Input::mouse.button = MouseBtn::NONE;
				break;
			}
			case SDL_MOUSEWHEEL:
			{
				// There's also Mouse wheel scroll left and right
				Input::mouse.wheel.x = (float) sdlEvent.wheel.x;
				Input::mouse.wheel.y = (float) sdlEvent.wheel.y;
				break;
			}
			default:
				Input::mouse.wheel.x = 0;
				Input::mouse.wheel.y = 0;
				break;
		}
	}

	SDL_Rect CameraModule::GetRectRelativeToCamera(SDL_Rect rect) noexcept
	{
		Vector2 mul =
		{
			(float) RSECore::GetScreenWidth()  / (float) m_MainCamera->m_Viewport.w,
			(float) RSECore::GetScreenHeight() / (float) m_MainCamera->m_Viewport.h
		};
		// Who will notice that sprites are overlapping with each other by a pixel?
		rect.w = (int) std::ceil(rect.w * mul.x);
		rect.h = (int) std::ceil(rect.h * mul.y);
		rect.x = (int) std::ceil((rect.x - m_MainCamera->m_Viewport.x) * mul.x);
		rect.y = (int) std::ceil((rect.y - m_MainCamera->m_Viewport.y) * mul.y);
		return rect;
	}
	
	Vector2 CameraModule::GetVectorRelativeToCamera(Vector2 vec) noexcept
	{
		Vector2 mul =
		{
			(float) RSECore::GetScreenWidth()  / (float) m_MainCamera->m_Viewport.w,
			(float) RSECore::GetScreenHeight() / (float) m_MainCamera->m_Viewport.h
		};
		vec.x = std::ceil((vec.x - m_MainCamera->m_Viewport.x) * mul.x);
		vec.y = std::ceil((vec.y - m_MainCamera->m_Viewport.y) * mul.y);
		return vec;
	}

	Vector2 CameraModule::GetMousePositionInWorld() noexcept
	{
		if (m_MainCamera)
		{
			Vector2 mouse = RSECore::GetMousePosition();
			mouse.x = (mouse.x * m_MainCamera->m_Scale.x) + m_MainCamera->m_Viewport.x;
			mouse.y = (mouse.y * m_MainCamera->m_Scale.y) + m_MainCamera->m_Viewport.y;
			return mouse;
		}
		return Vector2();
	}
}