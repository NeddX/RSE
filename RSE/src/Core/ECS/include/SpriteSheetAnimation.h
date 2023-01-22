#ifndef SPRITE_SHEET_ANIM_H
#define SPRITE_SHEET_ANIM_H

#include <sdafx.h>
#include "ECS.h"

namespace RSE
{
	// Forward declerations
	class Sprite;

	enum class AnimPlaybackMode
	{
		Forward,
		Reverse,
		PingPong
	};

	class SpriteSheetAnimation : public Component
	{
	private:
		int m_SpriteSheetIndex;
		int m_Frames;
		int m_Duration;
		bool m_Play;
		AnimPlaybackMode m_PlaybackMode;
		Sprite* m_SpriteComponent;

	public:
		SpriteSheetAnimation(int spriteSheetIndex, int frames, 
			int duration, AnimPlaybackMode mode = AnimPlaybackMode::Forward);

	public:
		void Play();
		void Stop();
		void SetSheetIndex(int index);
		void SetPlaybackMode(AnimPlaybackMode mode);

	public:
		void Init() override;
		void Update(float deltaTime) override;
	};
}

#endif // SPRITE_SHEET_ANIM_H