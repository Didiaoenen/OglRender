#pragma once

#include <OglTools/tinyxml2.h>

#include "Core_Serializer.h"

namespace Core
{
	class Core_ISerializable
	{
	public:

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) = 0;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) = 0;

		virtual ~Core_ISerializable() = default;
	};
}