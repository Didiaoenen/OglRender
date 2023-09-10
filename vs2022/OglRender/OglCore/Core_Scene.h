#pragma once

#include "Core_Actor.h"
#include "Core_ISerializable.h"

#include "Core_CModelRenderer.h"
#include "Core_CCamera.h"
#include "Core_CLight.h"

namespace Core
{
	class Core_Scene : public Core_ISerializable
	{
	public:
		struct FastAccessComponents
		{
			std::vector<Core_CModelRenderer*>	modelRenderers;
			std::vector<Core_CCamera*>			cameras;
			std::vector<Core_CLight*>			lights;
		};

		Core_Scene();

		~Core_Scene();

		void Play();

		bool IsPlaying() const;

		void Update(float p_deltaTime);

		void FixedUpdate(float p_deltaTime);

		void LateUpdate(float p_deltaTime);

		Core_Actor& CreateActor();

		Core_Actor& CreateActor(const std::string& p_name, const std::string& p_tag = "");

		bool DestroyActor(Core_Actor& p_target);

		void CollectGarbages();

		Core_Actor* FindActorByName(const std::string& p_name);

		Core_Actor* FindActorByTag(const std::string& p_tag);

		Core_Actor* FindActorByID(int64_t p_id);

		std::vector<std::reference_wrapper<Core_Actor>> FindActorsByName(const std::string& p_name);

		std::vector<std::reference_wrapper<Core_Actor>> FindActorsByTag(const std::string& p_tag);

		void OnComponentAdded(Core_AComponent& p_compononent);

		void OnComponentRemoved(Core_AComponent& p_compononent);

		std::vector<Core_Actor*>& GetActors();

		const FastAccessComponents& GetFastAccessComponents() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_root) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_root) override;

	private:
		int64_t m_availableID = 1;
		bool m_isPlaying = false;
		std::vector<Core_Actor*> m_actors;

		FastAccessComponents m_fastAccessComponents;
	};
}

