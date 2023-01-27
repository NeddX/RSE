#include "include/SpriteSheetAnimation.h"
#include "include/Sprite.h"
#include "../include/Core.h"

namespace Advres::RSE
{
	SpriteSheetAnimation::SpriteSheetAnimation(int spriteSheetIndex, int frames, int duration, AnimPlaybackMode mode) :
		m_SpriteSheetIndex(spriteSheetIndex), m_Frames(frames), m_Duration(duration), m_PlaybackMode(mode)
	{
	}

	void SpriteSheetAnimation::Play()
	{
		m_Play = true;
	}

	void SpriteSheetAnimation::Stop()
	{
		m_Play = false;
	}

	void SpriteSheetAnimation::SetSheetIndex(int index)
	{
		m_SpriteSheetIndex = index;
		m_SpriteComponent->m_CurrentSrcRect.y += m_SpriteSheetIndex * m_SpriteComponent->m_SrcRect.h;
	}

	void SpriteSheetAnimation::SetPlaybackMode(AnimPlaybackMode mode)
	{
		m_PlaybackMode = mode;
	}

	void SpriteSheetAnimation::Init()
	{
		m_SpriteComponent = this->parent->GetComponent<Sprite>();
		m_Play = false;
	}

	void SpriteSheetAnimation::Update(float deltaTime)
	{
		if (m_Play)
		{
			switch (m_PlaybackMode)
			{
				case AnimPlaybackMode::Forward:
				{
					int a = RSECore::GetMillisecondsPassed() / m_Duration % m_Frames;
					m_SpriteComponent->m_CurrentSrcRect.x = a * m_SpriteComponent->m_SrcRect.w + m_SpriteComponent->m_SrcRect.x;
					break;
				}
				case AnimPlaybackMode::Reverse:
				{
					break;
				}
				case AnimPlaybackMode::PingPong:
				{
					int a = RSECore::GetMillisecondsPassed() / m_Duration % m_Frames * 2;
					if (a >= m_Frames) a = m_Frames * 2 - a - 1;
					m_SpriteComponent->m_CurrentSrcRect.x = a * m_SpriteComponent->m_SrcRect.w + m_SpriteComponent->m_SrcRect.x;
					break;
				}
			}
		}
	}
}