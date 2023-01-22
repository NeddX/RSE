#include "include/Camera2DComponent.h"
#include "../include/Core.h"


namespace RSE
{
    bool d = false;

    Camera2DComponent::Camera2DComponent(Transform transform) : transform(transform)
    {
        m_ParentTransform = nullptr;
        m_Size = { (int)RSECore::GetScreenWidth(), (int)RSECore::GetScreenHeight() };
        m_Viewport = Rect(0, 0, (int)m_Size.x, (int)m_Size.y);
        m_Bounds = m_Size;
        m_Scale = { 1.0f, 1.0f };
    }

    void Camera2DComponent::Init()
    {
        m_ParentTransform = this->entity->GetComponent<TransformComponent>();
    }

    void Camera2DComponent::Update(float deltaTime)
    {
        static float time = 0.0f;
        m_Viewport.w = (int)(m_Size.x * m_Scale.x);
        m_Viewport.h = (int)(m_Size.y * m_Scale.y);
        m_Viewport.x = (int)(GetWorldTransform().position.x);
        m_Viewport.y = (int)(GetWorldTransform().position.y);

        // Ensure the camera doesn't go out of bounds
        // TODO: Add limits
        if      (m_Viewport.x < 0)              m_Viewport.x = 0;
        else if (m_Viewport.x > m_Bounds.x)     m_Viewport.x = RSECore::GetScreenWidth();
        if      (m_Viewport.y < 0)              m_Viewport.y = 0;
        else if (m_Viewport.y > m_Bounds.y)     m_Viewport.y = RSECore::GetScreenHeight();

        /*if (Input::IsKeyDown(KeyChar::B) && time * 1000 >= 100)
        {
            if (!d)
            {
                RSECore::SetViewportCamera2D(nullptr);
                d = true;
            }
            else
            {
                RSECore::SetViewportCamera2D(this);
                d = false;
            }
            time = 0.0f;
        }
        else
        {
            time += deltaTime;
        }*/
    }

    void Camera2DComponent::Render(float deltaTime)
    {
        auto a = m_Viewport.SDL();
        SDL_SetRenderDrawColor(RSECore::sdlRenderer, 0, 255, 0, 255);
        if (d) SDL_RenderDrawRect(RSECore::sdlRenderer, &a);
    }
}