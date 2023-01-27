#ifndef CAMERA_2D_COMPONENT_H
#define CAMERA_2D_COMPONENT_H

#include "ECS.h"
#include "TransformComponent.h"
#include "../../include/Vector2.h"
#include "../../include/Transform.h"
#include "../../include/Geometry.h"

namespace Advres::RSE
{
    // Forward declerations
    class RSECore;

    class Camera2DComponent : public Component
    {
        friend class RSECore;
        friend class CameraModule;

    public:
        Transform transform;

    private:
        Vector2 m_Size;
        Vector2 m_Scale;
        Vector2 m_Bounds;
        TransformComponent* m_ParentTransform;
        Rect m_Viewport = { 0, 0, 0, 0 };

    public:
        Camera2DComponent(Transform transform = Transform());

    public:
        inline void     SetBounds(Vector2 bounds)   noexcept { m_Bounds = bounds; }
        inline Vector2  GetBounds()                 noexcept { return m_Bounds; }
        inline void     SetScale(Vector2 scale)     noexcept { m_Scale = scale; }
        inline Vector2  GetScale()                  noexcept { return m_Scale; }
        inline int      GetWidth()                  noexcept { return m_Viewport.w; }
        inline int      GetHeight()                 noexcept { return m_Viewport.h; }

    public:
        void Init() override;
        void Update(float deltaTime) override;
        void Render(float deltaTime) override;
    };
}

#endif // CAMERA_COMPONENT_2D_H