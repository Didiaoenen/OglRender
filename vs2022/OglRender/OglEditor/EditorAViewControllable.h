#pragma once

#include "EditorAView.h"

namespace Editor
{
class EditorAViewControllable : public EditorAView
{
public:
	EditorAViewControllable
	(
		const std::string& pTitle,
		bool pOpened,
		bool pEnableFocusInputs = false
	);

	virtual void Update(float pDeltaTime) override;


};
}

