#include "pch.h"
#include "Renderer/Model.h"

namespace sr {
	void Model::CalculateRadius()
	{
		m_radius = 0.0f;

		//check for mesh radius
		for (const auto& mesh : m_meshes) {

			float radius = mesh.GetRadius();
			if (m_radius < radius) {
				m_radius = radius;
			}
		}
	}
}
