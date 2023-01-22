#ifndef CAMERA_2D_COMPONENT_H
#define CAMERA_2D_COMPONENT_H

#include "ECS.h"
#include "TransformComponent.h"
#include "../../include/Vector2.h"
#include "../../include/Transform.h"
#include "../../include/Geometry.h"

namespace RSE
{
    // Forward declerations
    class RSECore;

    class Camera2DComponent : public Component
    {
        friend class RSECore;

    public:
        Transform transform;

    private:
        Vector2 m_Size;
        Vector2 m_Scale;
        Vector2 m_Bounds;
        Rect m_Viewport = { 0, 0, 0, 0 };
        TransformComponent* m_ParentTransform;

    public:
        Camera2DComponent(Transform transform = Transform());

    public:
        inline Transform GetWorldTransform()
        {
            Transform gt;
            gt = transform + Transform(m_ParentTransform->position, Vector2(0, 0), m_ParentTransform->rotation);
            gt.scale *= m_ParentTransform->scale;
            return gt;
        }
        inline void     SetBounds(Vector2 bounds)   noexcept { m_Bounds = bounds; }
        inline Vector2  GetBounds()                 noexcept { return m_Bounds; }
        inline void     SetScale(Vector2 scale)     noexcept { m_Scale = scale; }
        inline Vector2  GetScale()                  noexcept { return m_Scale; }

    public:
        void Init() override;
        void Update(float deltaTime) override;
        void Render(float deltaTime) override;
    };
}

#endif // CAMERA_COMPONENT_2D_H