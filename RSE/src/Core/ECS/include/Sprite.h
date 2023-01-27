#ifndef SPRITE_H
#define SPRITE_H

#include <sdafx.h>
#include <SDL.h>
#include <SDL_image.h>

#include "ECS.h"
#include "TransformComponent.h"
#include "../../include/ResourceHandler.h"
#include "../../include/Texture2D.h"
#include "../../include/Transform.h"

namespace Advres::RSE
{
	// Forward declerations
	class SpriteSheetAnimation;

	enum class SpriteFlipState
	{
		None,
		Horizontal,
		Vertical
	};

	class Sprite : public Component
	{
		friend class Serializer;
		friend class SpriteSheetAnimation;

	public:
		TransformComponent* actorTransform;
		Transform transform;
		Texture2D* texture;
	
	private:
		SDL_Rect m_SrcRect, m_DestRect, m_CurrentSrcRect, m_CurrentDestRect;
		SpriteFlipState m_FlipState;
		RGBA m_Tint;

	public:
		Sprite(Texture2D* texture, Transform transform = Transform());

	public:
		inline void SetTexture(Texture2D* texture)
		{
			texture = texture;
			m_DestRect = { 0, 0, texture->GetWidth(), texture->GetHeight()};
		}
		inline void SetRenderRect(Rect srcRect, Rect destRect)
		{
			m_SrcRect = {  (int) srcRect.x, (int) srcRect.y, (int) srcRect.w, (int) srcRect.h };
			m_CurrentSrcRect = m_SrcRect;
			m_DestRect = { (int) destRect.x, (int) destRect.y, (int) destRect.w, (int) destRect.h };
		}
		inline void SetFlipState(SpriteFlipState state)
		{
			m_FlipState = state;
		}
		inline Rect GetRenderRect()
		{
			return Rect(m_DestRect);
		}
		inline void SetTint(RGBA colour)
		{
			m_Tint = colour;
		}

	public:
		void Serialize(YAML::Emitter& out) const override;

	public:
		virtual void Init() override;
		virtual void Update(float deltaTime) override;
		virtual void Render(float deltaTime) override;
	};
}

#endif // SPRITE_H