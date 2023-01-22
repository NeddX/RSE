#include "include/Sprite.h"
#include "../include/Core.h"
#include "include/SpriteSheetAnimation.h"

namespace RSE
{
	Sprite::Sprite(Texture2D* texture, Transform transform) : texture(texture), transform(transform)
	{
		m_FlipState = SpriteFlipState::None;
	}

	void Sprite::Serialize(YAML::Emitter& out) const
	{
		out << YAML::BeginMap;
		out << YAML::Key << "SpriteComponent" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "actorTransform" << YAML::Value << "owner.GetComponent<TransformComponent>";
		out << YAML::Key << "transform" << YAML::Value;
		Serializer::Serialize(transform, out);
		out << YAML::Key << "texture" << YAML::Value;
		Serializer::Serialize(Resources::GetResource<Texture2D>(texture->GetID()).get(), out);
		out << YAML::Key << "m_SrcRect" << YAML::Value;
		Serializer::Serialize(m_SrcRect, out);
		out << YAML::Key << "m_DestRect" << YAML::Value;
		Serializer::Serialize(m_DestRect, out);
		out << YAML::Key << "m_CurrentDestRect" << YAML::Value;
		Serializer::Serialize(m_CurrentDestRect, out);
		out << YAML::EndMap;
		out << YAML::EndMap;
	}

	void Sprite::Init()
	{
		actorTransform = entity->GetComponent<TransformComponent>();
		m_SrcRect = { 0, 0, texture->GetWidth(), texture->GetHeight() };
		m_CurrentSrcRect = m_SrcRect;

		m_DestRect.w = texture->GetWidth();
		m_DestRect.h = texture->GetHeight();
	}

	void Sprite::Update(float deltaTime)
	{
		m_CurrentDestRect.x = (int) actorTransform->position.x + (int) transform.position.x;
		m_CurrentDestRect.y = (int) actorTransform->position.y + (int) transform.position.y;
		m_CurrentDestRect.w = m_DestRect.w * (int) transform.scale.x * (int) actorTransform->scale.x;
		m_CurrentDestRect.h = m_DestRect.h * (int) transform.scale.y * (int) actorTransform->scale.y;
	}

	void Sprite::Render(float deltaTime)
	{
		// Don't bother rendering if the rectangle is not in view of the camera.
		if (RSECore::IsInView({ m_CurrentDestRect.x, m_CurrentDestRect.y, m_CurrentDestRect.w, m_CurrentDestRect.h }))
		{
			RGBA prev;
			SDL_GetTextureColorMod(texture->m_SdlTex, &prev.r, &prev.g, &prev.b);
			SDL_SetTextureColorMod(texture->m_SdlTex, m_Tint.r, m_Tint.g, m_Tint.b);
			RSECore::DrawTexture(texture, &m_CurrentSrcRect, &m_CurrentDestRect,
				255, transform.rotation, nullptr, (SDL_RendererFlip)m_FlipState);
			SDL_SetTextureColorMod(texture->m_SdlTex, prev.r, prev.g, prev.b);
		}
	}
}