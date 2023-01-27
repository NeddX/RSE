#include "include/Texture2D.h"
#include "include/Core.h"

namespace Advres::RSE
{
	Texture2D::Texture2D(SDL_Texture* texture, int w, int h, size_t id) : m_SdlTex(texture), m_W(w), m_H(h)
	{
		this->id = id;
	}

	Texture2D::~Texture2D()
	{
		SDL_DestroyTexture(m_SdlTex);
	}
}