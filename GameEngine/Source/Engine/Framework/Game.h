#pragma once
#include "Framework/Scene.h"
#include <memory>

namespace sr {

	class Game
	{
	public:
		Game();
		virtual ~Game();
		

		virtual bool Initialize() { return true; }
		virtual bool Shutdown() { return true; }

		virtual void Update(float dt, float width, float height);
		virtual void Draw(class Renderer& renderer);

		void SetScene(std::unique_ptr<Scene> scene);
	protected:
		std::unique_ptr<Scene> m_scene = nullptr;


	};
}