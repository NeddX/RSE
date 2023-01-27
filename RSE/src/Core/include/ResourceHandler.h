#ifndef RESOURCE_HANDLER_H
#define RESOURCE_HANDLER_H

#include <sdafx.h>

#include "RException.h"

namespace Advres::RSE
{
	// Forward declerations
	//class Texture2D;

	class IResource
	{
	protected:
		size_t id = 0;

	public:
		IResource() = default;
		virtual ~IResource() = default;

	public:
		constexpr size_t inline GetID() const noexcept { return id; }
	};

	class Resources
	{
	private:
		static std::unordered_map<size_t, std::shared_ptr<IResource>> m_Resources;

	public:
		template<typename T, typename = std::enable_if<std::is_base_of<IResource, T>::value>>
		static std::shared_ptr<T> Load(const char* file);

	public:
		template<typename T>
		static const std::shared_ptr<T> GetResource(size_t id)
		{
			auto it = m_Resources.find(id);
			if (it != m_Resources.end()) return std::static_pointer_cast<T>(it->second);
			else throw RException(fmt::format("Hash ID {} was not present in the Resource Pool.", id).c_str());
		}
		static std::unordered_map<size_t, std::shared_ptr<IResource>>& GetAllResources()
		{
			return m_Resources;
		}
	};
}

#endif // RESOURCE_HANDLER_H