#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Render_Mesh.h"

namespace Render
{
	class Render_ModelLoader;

	class Render_Model
	{
		friend class Render_ModelLoader;

	public:
		const std::vector<Render_Mesh*>& GetMeshes() const;

		const std::vector<std::string>& GetMaterialNames() const;

		const Render_BoundingSphere& GetBoundingSphere() const;

	private:
		Render_Model(const std::string& pPath);
		~Render_Model();

		void ComputeBoundingSphere();

	public:
		const std::string path;

	private:
		std::vector<Render_Mesh*> m_meshes;
		std::vector<std::string> m_materialNames;

		Render_BoundingSphere m_boundingSphere;
	};
}

