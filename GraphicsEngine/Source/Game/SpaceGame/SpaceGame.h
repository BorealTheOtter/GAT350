#pragma once
#include "Framework/Game.h"
#include "Renderer/Text.h"


class SpaceGame : public sr::Game {

public:
	enum class GameState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		Dead,
		GameOver
	};

	SpaceGame() = default;

	bool Initialize() override;

	void Update(float dt, float width, float height) override;
	void Draw(class sr::Renderer& renderer) override;

	void OnPlayerDead();
	void AddPoints(int points) { m_score += points; }

	void SetGameState(GameState gs) { m_gameState = gs; }

private:
	void SpawnPlayer();
	void SpawnEnemy();

	int m_score = 0;
	int m_lives = 0;

	float m_spawnTimer = 0.0f;
	float m_spawnTime = 5.0f;

	float m_stateTimer = 0.0f;

	GameState m_gameState = GameState::Title;

	sr::res_t<sr::Font> m_menuFont;
	sr::res_t<sr::Font> m_gameFont;

	//sr::Font* m_menuFont{ nullptr };
	sr::Text* m_titleText{ nullptr };
	sr::Text* m_gameOverText{ nullptr };
	
	//sr::Font* m_gameFont{ nullptr };
	sr::Text* m_scoreText{ nullptr };
	sr::Text* m_livesText{ nullptr };

	
};