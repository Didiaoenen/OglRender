#pragma once

#include <stack>
#include <string>

#include "base/OGL_Base.h"
#include "interface/OGL_ISceneManager.h"

namespace OGL
{
class OGL_SceneManager : public OGL_ISceneManager
{
public:
	OGL_SceneManager() = default;
	~OGL_SceneManager() = default;

	bool Initialize() override;
	void Finalize() override;

	void Tick() override;

	bool LoadScene(const std::string& sceneName) override;

	uint64_t GetSceneRevision() const override { return mSceneRevision; }

	const Ref<OGL_Scene> GetSceneForRendering() const override;

	void ResetScene() override { mSceneRevision++; };

protected:
	std::stack<Ref<OGL_Scene>> mScenes;
	uint64_t mSceneRevision{ 0 };
};
}