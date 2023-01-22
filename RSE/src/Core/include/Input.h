#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <variant>
#include <algorithm>

//#include "Core.h"
#include "Vector2.h"

namespace RSE
{
	// Forward declerations
	//class Vector2;

	enum class KeyChar : uint32_t
	{
		NUM_0		= SDLK_0,
		NUM_1		= SDLK_1,
		NUM_2		= SDLK_2,
		NUM_3		= SDLK_3,
		NUM_4		= SDLK_4,
		NUM_5		= SDLK_5,
		NUM_6		= SDLK_6,
		NUM_7		= SDLK_7,
		NUM_8		= SDLK_8,
		NUM_9		= SDLK_9,

		A			= SDLK_a,
		B			= SDLK_b,
		C			= SDLK_c,
		D			= SDLK_d,
		E			= SDLK_e,
		F			= SDLK_f,
		G			= SDLK_g,
		H			= SDLK_h,
		I			= SDLK_i,
		J			= SDLK_j,
		K			= SDLK_k,
		L			= SDLK_l,
		M			= SDLK_m,
		N			= SDLK_n,
		O			= SDLK_o,
		P			= SDLK_p,
		Q			= SDLK_q,
		R			= SDLK_r,
		S			= SDLK_s,
		T			= SDLK_t,
		U			= SDLK_u,
		V			= SDLK_v,
		W			= SDLK_w,
		X			= SDLK_x,
		Y			= SDLK_y,
		Z			= SDLK_z,

		RETURN		= SDLK_RETURN,
		ESCAPE		= SDLK_ESCAPE,
		TAB			= SDLK_TAB,
		SPACE		= SDLK_SPACE,

		ARROW_UP	= SDLK_UP,
		ARROW_DOWN	= SDLK_DOWN,
		ARROW_LEFT	= SDLK_LEFT,
		ARROW_RIGHT = SDLK_RIGHT
	};

	enum class MouseBtn : uint16_t
	{
		NONE = 0,
		LMB	 = SDL_BUTTON_LEFT,
		MMB,
		RMB
	};

	struct MouseState
	{
		MouseBtn button;
		Vector2 wheel;
	};

	struct KeyBind
	{
		std::vector<KeyChar> keys;
	};

	struct AxisBind
	{
		KeyChar pos, neg;
	};

	class Input
	{
	private:
		static std::unordered_map<std::string, std::variant<KeyBind, AxisBind>> m_Actions;
	
	public:
		static std::unordered_map<uint32_t, bool> keys;
		static MouseState mouse;

		static void Init()
		{
			// Numbers
			keys[SDLK_0]		= false;
			keys[SDLK_1]		= false;
			keys[SDLK_2]		= false;
			keys[SDLK_3]		= false;
			keys[SDLK_4]		= false;
			keys[SDLK_5]		= false;
			keys[SDLK_6]		= false;
			keys[SDLK_7]		= false;
			keys[SDLK_8]		= false;
			keys[SDLK_9]		= false;

			// Letters
			keys[SDLK_a]		= false;
			keys[SDLK_b]		= false;
			keys[SDLK_c]		= false;
			keys[SDLK_d]		= false;
			keys[SDLK_e]		= false;
			keys[SDLK_f]		= false;
			keys[SDLK_g]		= false;
			keys[SDLK_h]		= false;
			keys[SDLK_i]		= false;
			keys[SDLK_j]		= false;
			keys[SDLK_k]		= false;
			keys[SDLK_l]		= false;
			keys[SDLK_m]		= false;
			keys[SDLK_n]		= false;
			keys[SDLK_o]		= false;
			keys[SDLK_p]		= false;
			keys[SDLK_q]		= false;
			keys[SDLK_r]		= false;
			keys[SDLK_s]		= false;
			keys[SDLK_t]		= false;
			keys[SDLK_u]		= false;
			keys[SDLK_v]		= false;
			keys[SDLK_w]		= false;
			keys[SDLK_x]		= false;
			keys[SDLK_y]		= false;
			keys[SDLK_z]		= false;

			// Special keys i guess?
			keys[SDLK_RETURN]	= false;
			keys[SDLK_ESCAPE]	= false;
			keys[SDLK_TAB]		= false;
			keys[SDLK_SPACE]	= false;
			
			// Arrows
			keys[SDLK_UP]		= false;
			keys[SDLK_DOWN]		= false;
			keys[SDLK_LEFT]		= false;
			keys[SDLK_RIGHT]	= false;
		}

	public:
		template <typename T>
		static inline void AddAction(const std::string& actionName, T bindType)
		{
			m_Actions[actionName] = bindType;
		}
		static inline std::unordered_map<std::string, std::variant<KeyBind, AxisBind>>* GetAllActions()
		{
			return &m_Actions;
		}
		static inline bool IsActionActive(const std::string& actionName) 
		{
			const auto it = m_Actions.find(actionName);
			if (it == m_Actions.end()) return false;

			return std::visit([](const auto& bindType) 
				{
					if constexpr (std::is_same_v<std::decay_t<decltype(bindType)>, KeyBind>) 
					{
						return std::any_of(bindType.keys.begin(), bindType.keys.end(), [&](const auto& k) { return keys[ (uint16_t) k]; });
					}
					else 
					{
						return false;
					}
				}, it->second);
		}
		static inline float GetActionAxis(const std::string& actionName) 
		{
			auto it = m_Actions.find(actionName);
			if (it == m_Actions.end()) return 0;

			AxisBind axisBind = std::get<AxisBind>(it->second);
			float axis = 0;
			if (keys[(uint32_t) axisBind.pos]) axis += 1;
			if (keys[(uint32_t) axisBind.neg]) axis += -1;
			return axis;
		}
		static inline bool IsKeyDown(KeyChar key)
		{
			return keys[(uint32_t) key];
		}
	};
}

#endif // INPUT_H