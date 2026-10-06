#include "pch.h"
#include "Framework/Game.h"
#include "Framework/Scene.h"

namespace sr {
	Game::Game() = default;
	Game::~Game() = default;

	void Game::Update(float dt, float width, float height){
		m_scene->Update(dt, width, height);
	}

	void Game::Draw(Renderer& renderer){
		m_scene->Draw(renderer);
	}
	void Game::SetScene(std::unique_ptr<Scene> scene)
	{
		m_scene = std::move(scene);
	}
}
