#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <sdafx.h>
#include <SDL_image.h>

#include "Geometry.h"
#include "ResourceHandler.h"

namespace RSE
{
	// Forward declerations
	class RSECore;
	class Sprite;
	class TileMapComponent;

	// Wrapper around SDL_Texture
	class Texture2D : public IResource
	{
		friend class Resources;
		friend class RSECore;
		friend class Sprite;
		friend class TileMapComponent;

	private:
		SDL_Texture* m_SdlTex;
		int m_W, m_H;

	public:
		Texture2D(SDL_Texture* texture, int w, int h, size_t id);
		~Texture2D();

	public:
		constexpr inline int GetWidth()  noexcept { return m_W; }
		constexpr inline int GetHeight() noexcept { return m_H; }
	};
}

#endif // TEXTURE2D_H