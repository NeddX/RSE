#ifndef CORE_H
#define CORE_H

#include <sdafx.h>

#include "RException.h"
#include "Vector2.h"
#include "Texture2D.h"
#include "Input.h"
#include "Transform.h"
#include "Geometry.h"
#include "../ECS/include/ECS.h"
#include "../ECS/include/Camera2DComponent.h"

#ifdef RCLRW_H
#include "../../RCLRW/src/include/RCLRW.h"
#endif

namespace Advres::RSE
{
	// Forward declerations
	class RSECore;
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

	// Safe static wrapper around Camera2D
	class CameraModule
	{
		friend class RSECore;
#ifdef RCLRW_H
		friend class Net::RSEFramework;
#endif

	private:
		static const Camera2DComponent* m_MainCamera;

	public:
		static 			SDL_Rect	GetRectRelativeToCamera(SDL_Rect rect)				noexcept;
		static 			Vector2f		GetVectorRelativeToCamera(Vector2f vec)				noexcept;
		static			Vector2f		GetMousePositionInWorld()							noexcept;
		static			void		SetActiveCamera(const Camera2DComponent* camera)	noexcept { m_MainCamera = camera; }
		static inline	bool		Active()											noexcept { return (m_MainCamera) ? true : false; }
	};

	class RSECore
	{
#ifdef RCLRW_H
		friend class Net::RSEFramework;
#endif

	private:
		uint64_t m_FrameCount;
		int m_Fps;
		int m_TargetFps;
		int m_FrameDelay;
		uint32_t m_FrameStart;
		bool m_Running;
		const char* m_WindowTitle;
		void (*m_RenderMethod)(float);
		void (*m_UpdateMethod)(float);

	private:
		static std::chrono::system_clock::time_point m_Tp1, m_Tp2;
		static std::chrono::high_resolution_clock::time_point m_StartTp;
		static int m_ScreenWidth;
		static int m_ScreenHeight; 
		static std::vector<Rect> m_DebugRects;
		static std::vector<std::pair<Vector2f, Vector2f>> m_DebugLines;

	public:
		static SDL_Window* sdlWindow;
		static SDL_Renderer* sdlRenderer;
		static SDL_Event sdlEvent;
		static SDL_GLContext glContext;
		static std::vector<BoxCollider2D*> colliders;
		static std::shared_ptr<EntityManager> entityManager;
		static std::unique_ptr<EntityManager> UIManager;
		static bool disableCamera;
		static CameraModule* camera;

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
		static Vector2f GetMousePosition()
		{
			int mX, mY;
			SDL_GetMouseState(&mX, &mY);
			Vector2f vec = { mX, mY };
			return vec;
		}
		static inline void DrawDebugRect(Rect rect)
		{
			m_DebugRects.push_back(CameraModule::GetRectRelativeToCamera(rect.SDL()));
		}
		static inline void DrawDebugLine(std::pair<Vector2f, Vector2f> line)
		{
			line.first = CameraModule::GetVectorRelativeToCamera(line.first);
			line.second = CameraModule::GetVectorRelativeToCamera(line.second);
			m_DebugLines.push_back(line);
		}
		static inline float GetDeltaTime()
		{
			m_Tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = m_Tp2 - m_Tp1;
			m_Tp1 = m_Tp2;
			return elapsedTime.count();
		}
		static bool IsInView(Rect rect)	noexcept;
					static	inline	int							GetScreenWidth()										noexcept	{ return m_ScreenWidth; }
					static	inline	int							GetScreenHeight()										noexcept	{ return m_ScreenHeight; }
					static	inline	uint64_t					GetMillisecondsPassed()									noexcept	{ return (uint64_t) std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_StartTp).count(); }
					static	inline	Entity*						GetEntityByTag(const std::string& tag)					noexcept	{ return entityManager->GetEntityByTag(tag); }
					static	inline	Entity*						AddEntity()												noexcept	{ return entityManager->AddEntity(); }
					static	inline	void						DisposeEntity(Entity* parent)							noexcept	{ entityManager->DisposeEntity(parent); }
					static	inline	Entity*						AddUIComponent()										noexcept	{ return UIManager->AddEntity(); }
		constexpr			inline	int							GetFrameRate()						const							{ return m_Fps; }
		constexpr			inline	uint64_t					GetFrameCount()						const							{ return m_FrameCount; }
		constexpr			inline	const char*					GetWindowTitle()					const				noexcept	{ return m_WindowTitle; }
							inline	void						SetWindowTitle(const char* title)						noexcept	{ m_WindowTitle = title; SDL_SetWindowTitle(sdlWindow, title); }
		constexpr			inline	bool						GetRunning()						const				noexcept	{ return m_Running; }

	public:
		static inline void DrawTexture(const Texture2D* texture, SDL_Rect* const srcR, SDL_Rect* const destR, int opacity = 255,
			float angle = 0.0f, SDL_Point* centrePoint = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE)
		{
			SDL_Rect* dest_rx = (SDL_Rect*) destR;
			if (camera->Active() && !disableCamera)
			{
				// TODO: Improve
				*dest_rx = camera->GetRectRelativeToCamera(*dest_rx);
			}
			SDL_SetTextureAlphaMod(texture->m_SdlTex, opacity);
			SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
			SDL_RenderCopyEx(sdlRenderer, texture->m_SdlTex, srcR, dest_rx, angle, centrePoint, flip);
			SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_NONE);
		}
		static inline void DrawTextureOnScreen(const Texture2D* texture, SDL_Rect* const srcR, SDL_Rect* const destR, int opacity = 255,
			float angle = 0.0f, SDL_Point* centrePoint = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE)
		{
			// Exact same function as DrawTexture, difference being that we don't draw relative to the camera
			SDL_SetTextureAlphaMod(texture->m_SdlTex, opacity);
			SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
			SDL_RenderCopyEx(sdlRenderer, texture->m_SdlTex, srcR, destR, angle, centrePoint, flip);
			SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_NONE);
		}
		static inline void DrawRect(const Rect rect, uint8_t r = 0, uint8_t g = 255, uint8_t b = 0, uint8_t a = 255)
		{
			SDL_Rect dest_cp = rect.SDL();
			SDL_SetRenderDrawColor(sdlRenderer, r, g, b, a);
			if (camera->Active() && !disableCamera)
			{
				// TODO: Improve
				dest_cp = camera->GetRectRelativeToCamera(dest_cp);
			}
			SDL_RenderDrawRect(sdlRenderer, &dest_cp);
		}
		static inline void DrawLine(Vector2f src, Vector2f dest, uint8_t r = 0, uint8_t g = 255, uint8_t b = 0, uint8_t a = 255)
		{
			if (camera->Active() && !disableCamera)
			{
				src = camera->GetVectorRelativeToCamera(src);
				dest = camera->GetVectorRelativeToCamera(dest);
			}
			SDL_SetRenderDrawColor(sdlRenderer, r, g, b, a);
			SDL_RenderDrawLine(sdlRenderer, (int) src.x, (int) src.y, (int) dest.x, (int) dest.y);
		}

	private:
		void EngineThread();
		void EventHandler();
	};
}

#endif // CORE_H