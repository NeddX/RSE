
#include "include/ResourceHandler.h"
#include "Core.h"
#include "include/Texture2D.h"

namespace Advres::RSE
{
	std::unordered_map<size_t, std::shared_ptr<IResource>> Resources::m_Resources;

	template<typename T, typename>
	std::shared_ptr<T> Resources::Load(const char* file)
	{
		static_assert("Type not supported.");
		return std::shared_ptr<T>();
	}

	template<>
	std::shared_ptr<Texture2D> Resources::Load(const char* file)
	{
		std::ifstream fs(file);
		if (fs.is_open())
		{ 
			SDL_Surface* tmp_surf = IMG_Load(file);
			SDL_Texture* sdl_tex = SDL_CreateTextureFromSurface(RSECore::sdlRenderer, tmp_surf);
			size_t id = xstdf::crypto::FNV1aHash(file);
			std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(sdl_tex, tmp_surf->w, tmp_surf->h, id);
			m_Resources[id] = std::static_pointer_cast<IResource>(texture);
			fs.close();
			fmt::println("[ResourceHandler] >> File: '{}' ID: {}", file, id);
			return texture;
		}
		else
		{
			fs.close();
			throw RException(fmt::format("File '{}' supplied to Load() does not exist.", file).c_str());
			return std::shared_ptr<Texture2D>();
		}
	}
}