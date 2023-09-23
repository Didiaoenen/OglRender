#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Maths
{
	template<typename T>
	class Maths_GlmExt
	{
	public:
		static T Up()
		{
			return T(0.f, 1.f, 0.f);
		}

		static T Forward()
		{
			return T(0.f, 0.f, 1.f);
		}

		static T Right()
		{
			return T(1.f, 0.f, 0.f);
		}
	};
}


namespace glm
{
	template<typename T>
	T up()
	{
		return Maths::Maths_GlmExt<T>::Up();
	}

	template<typename T>
	T forward()
	{
		return Maths::Maths_GlmExt<T>::Forward();
	}

	template<typename T>
	T right()
	{
		return Maths::Maths_GlmExt<T>::Right();
	}
}