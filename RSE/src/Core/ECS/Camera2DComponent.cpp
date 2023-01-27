#include "include/Camera2DComponent.h"
#include "../include/Core.h"


namespace Advres::RSE
{
    bool d = false;

    Camera2DComponent::Camera2DComponent(Transform transform) : transform(transform)
    {
        m_ParentTransform = nullptr;
        m_Size = { (int)RSECore::GetScreenWidth(), (int)RSECore::GetScreenHeight() };
        m_Viewport = Rect(0, 0, (int)m_Size.x, (int)m_Size.y);
        m_Bounds = m_Size;
        m_Scale = { 1.0f, 1.0f };

        // By default all newly created cameras activate themselves automatically
        RSECore::camera->SetActiveCamera(this);
    }

    void Camera2DComponent::Init()
    {
        m_ParentTransform = this->parent->GetComponent<TransformComponent>();
    }

    void Camera2DComponent::Update(float deltaTime)
    {
#ifdef _DEBUG
        static float time = 0.0f;
#endif

        m_Viewport.w = (int)(m_Size.x * m_Scale.x);
        m_Viewport.h = (int)(m_Size.y * m_Scale.y);
        m_Viewport.x = (int)(m_ParentTransform->position.x + (transform.position.x * m_Scale.x));
        m_Viewport.y = (int)(m_ParentTransform->position.y + (transform.position.y * m_Scale.y));

        // Ensure the camera doesn't go out of bounds
        // TODO: Add limits
        if      (m_Viewport.x < 0)              m_Viewport.x = 0;
        else if (m_Viewport.x > m_Bounds.x)     m_Viewport.x = RSECore::GetScreenWidth();
        if      (m_Viewport.y < 0)              m_Viewport.y = 0;
        else if (m_Viewport.y > m_Bounds.y)     m_Viewport.y = RSECore::GetScreenHeight();

#ifdef _DEBUG
        // Debug: Disable camera and draw the borders of the  camera
        if (Input::IsKeyDown(Key::B) && time * 1000 >= 300)
        {
            if (!d)
            {
                RSECore::disableCamera = true;
                d = true;
            }
            else
            {
                RSECore::disableCamera = false;
                d = false;
            }
            time = 0.0f;
        }
        else
        {
            time += deltaTime;
        }
#endif
    }

    void Camera2DComponent::Render(float deltaTime)
    {
#ifdef _DEBUG
        auto a = m_Viewport.SDL();
        if (d)
        {
            SDL_SetRenderDrawColor(RSECore::sdlRenderer, 0, 0, 255, 255);
            SDL_RenderDrawRect(RSECore::sdlRenderer, &a);
        }
#endif
    }
}