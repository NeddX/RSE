#ifndef CORE_H
#define CORE_H

// STL
#include <sdafx.h>
#include <fcntl.h>
#include <corecrt_io.h>
#include <SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "RException.h"
#include "Vector2.h"
#include "Texture2D.h"
#include "Map.h"
#include "Input.h"
#include "Transform.h"
#include "Geometry.h"
#include "../ECS/include/ECS.h"
#include "../ECS/include/Camera2DComponent.h"

namespace RSE
{
	// Forward declerations
	class BoxCollider2D;

	enum class RSEStatus : uint32_t
	{
		RSE_SDL_INIT_FAILURE,
		RSE_SDL_WINDOW_INIT_FAILURE,
		RSE_SDL_RENDERER_INIT_FAILURE,
		RSE_OK
	};

	enum class RSEWindowState : uint8_t
	{
		FULLSCREEN,
		WINDOWED
	};

	class RSECore
	{
	private:
		uint64_t m_FrameCount;
		int m_Fps;
		int m_TargetFps;
		int m_FrameDelay;
		uint32_t m_FrameStart;
		std::chrono::system_clock::time_point m_Tp1, m_Tp2;
		bool m_Running;
		const char* m_WindowTitle;

	private:
		static std::chrono::high_resolution_clock::time_point m_StartTp;
		static const Camera2DComponent* m_Camera;
		static int m_ScreenWidth;
		static int m_ScreenHeight;

	public:
		static SDL_Window* sdlWindow;
		static SDL_Renderer* sdlRenderer;
		static SDL_Event sdlEvent;
		static SDL_GLContext glContext;
		static std::vector<BoxCollider2D*> colliders;
		static std::shared_ptr<EntityManager> entityManager;
		static std::unique_ptr<EntityManager> uiManager;

	public:
		RSECore();
		virtual ~RSECore();

	public:
		RSEStatus Init(
			const char* m_WindowTitle,
			int m_ScreenWidth,
			int m_ScreenHeight,
			int fpsLimit = 60,
			int x = SDL_WINDOWPOS_CENTERED,
			int y = SDL_WINDOWPOS_CENTERED,
			RSEWindowState windowState = RSEWindowState::WINDOWED);

	protected:
		virtual void Start();
		virtual void Update(float elapsedTime);
		virtual void Render(float elapsedTime);

	// Getters and Setters
	public:
		static Vector2 GetMousePos()
		{
			Vector2 vec = { 0, 0 };
			SDL_GetMouseState((int*) &vec.x, (int*) &vec.y);
			if (m_Camera)
			{
				vec.x = (vec.x + m_Camera->m_Viewport.x);// * m_Camera->scale;
				vec.y = (vec.y + m_Camera->m_Viewport.y);// * m_Camera->scale;
			}
			return vec;
		}
		inline float GetDeltaTime()
		{
			m_Tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = m_Tp2 - m_Tp1;
			m_Tp1 = m_Tp2;
			return elapsedTime.count();
		}
		static bool IsInView(Rect rect)	noexcept;
		constexpr	static	inline	int			GetScreenWidth()										noexcept	{ return m_ScreenWidth; }
		constexpr	static	inline	int			GetScreenHeight()										noexcept	{ return m_ScreenHeight; }
		constexpr	static	inline	uint64_t	GetMillisecondsPassed()									noexcept	{ return (uint64_t) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_StartTp).count(); }
					static	inline	Entity*		GetEntityByTag(const std::string& tag)					noexcept	{ return entityManager->GetEntityByTag(tag); }
					static	inline	Entity*		AddEntity()												noexcept	{ return entityManager->AddEntity(); }
					static	inline	void		DisposeEntity(Entity* entity)							noexcept	{ entityManager->DisposeEntity(entity); }
					static	inline	Entity*		AddUIComponent()										noexcept	{ return uiManager->AddEntity(); }
					static			void		SetViewportCamera2D(const Camera2DComponent* camera)	noexcept	{ m_Camera = camera; }
		constexpr			inline	int			GetFrameRate()						const							{ return m_Fps; }
		constexpr			inline	uint64_t	GetFrameCount()						const							{ return m_FrameCount; }
		constexpr			inline const char*	GetWindowTitle()					const				noexcept	{ return m_WindowTitle; }
							inline void			SetWindowTitle(const char* title)						noexcept	{ m_WindowTitle = title; SDL_SetWindowTitle(sdlWindow, title); }
		constexpr			inline bool			GetRunning()						const				noexcept	{ return m_Running; };
	
	public:
		static inline void DrawTexture(const Texture2D* texture, const SDL_Rect* srcR, const SDL_Rect* destR, int opacity = 255,
			float angle = 0.0f, SDL_Point* centrePoint = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE)
		{
			SDL_Rect* destRX = (SDL_Rect*) destR;
			if (m_Camera)
			{
				Vector2 mul =
				{
					(float) GetScreenWidth()  / (float) m_Camera->m_Viewport.w,
					(float) GetScreenHeight() / (float) m_Camera->m_Viewport.h
				};
				// Who will notice that sprites are overlapping with each other by a pixel?
				destRX->w = 1 + (int)(destRX->w * mul.x);
				destRX->h = 1 + (int)(destRX->h * mul.y);
				destRX->x = 1 + (int)((destRX->x - m_Camera->m_Viewport.x) * mul.x);
				destRX->y = 1 + (int)((destRX->y - m_Camera->m_Viewport.y) * mul.y);
			}
			SDL_SetTextureAlphaMod(texture->m_SdlTex, opacity);
			SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
			SDL_RenderCopyEx(sdlRenderer, texture->m_SdlTex, srcR, destRX, angle, centrePoint, flip);
			SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_NONE);
		}

	private:
		void EngineThread();
		void EventHandler();
	};
}

#endif // CORE_H