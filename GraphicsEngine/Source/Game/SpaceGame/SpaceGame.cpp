#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Assets.h"
#include "Resources/ResourceManager.h"
#include "Engine.h"

#include "Serialization/Json.h"

#include <memory>



bool SpaceGame::Initialize()
{
	Game::Initialize();

	sr::SetWorkingDirectory("SpaceGame");

	sr::Factory::Instance().Register<Player>("Player");
	sr::Factory::Instance().Register<Enemy>("Enemy");
	sr::Factory::Instance().Register<Bullet>("Bullet");

	sr::SetSeed();

	m_scene = std::make_unique<sr::Scene>();
	m_scene->SetGame(this);
	m_scene->Load("Data/stupid.json"); 

	

	sr::Engine::Get().GetAudio().AddSound("explosion", "Sounds/snd_badexplosion.wav");
	sr::Engine::Get().GetAudio().AddSound("shoot", "Sounds/snd_bomb.wav");

	m_menuFont = sr::Resources().GetWithID<sr::Font>("MenuFont","Fonts/gameFont.ttf", 64);
	m_gameFont = sr::Resources().GetWithID<sr::Font>("GameFont","Fonts/gameFont.ttf", 32);

	m_titleText = new sr::Text(m_menuFont);
	m_titleText->Create(sr::Engine::Get().GetRenderer(), "Space Shooter", sr::Vector3{ 1.0f, 1.0f, 1.0f });
	m_gameOverText = new sr::Text(m_menuFont);
	m_gameOverText->Create(sr::Engine::Get().GetRenderer(), "Game Over", sr::Vector3{ 1.0f, 1.0f, 1.0f });

	m_scoreText = new sr::Text(m_gameFont);
	m_livesText = new sr::Text(m_gameFont);
	return true;
}

void SpaceGame::Update(float dt, float width, float height){
	switch (m_gameState)
	{
	case SpaceGame::GameState::Title: {
		if (sr::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) m_gameState = GameState::StartGame;
		break;
	}
	case SpaceGame::GameState::StartGame: {
		m_score = 0;
		m_lives = 3;
		m_gameState = GameState::StartLevel;
		break;
	}
	case SpaceGame::GameState::StartLevel: {
		m_stateTimer -= dt;
		if (m_stateTimer <= 0.0f) {
			m_scene->ClearActors();
			SpawnPlayer();
			m_gameState = GameState::Game;
		}
		break;
	}
	case SpaceGame::GameState::Game:
		m_spawnTimer -= dt;
		if (m_spawnTimer <= 0.0f) {
			for (int i = 0; i < sr::RandomInt(1, 3); ++i) {
				SpawnEnemy();
			}
			m_spawnTimer = m_spawnTime;
			m_spawnTime -= 0.05f;
		}

		if (sr::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_Y)) {
			for (int i = 0; i < 1000; ++i) {
				SpawnEnemy();
			}
		}
		break;
	case SpaceGame::GameState::Dead:
		OnPlayerDead();
		m_stateTimer = 3.0f;
		break;
		
	case SpaceGame::GameState::GameOver:
		m_scene->ClearActors();
		if (sr::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) m_gameState = GameState::Title;
		break;
	default:
		break;
	}
	
	Game::Update(dt, width, height);
}

void SpaceGame::Draw(class sr::Renderer& renderer){
	switch (m_gameState)
	{
	case SpaceGame::GameState::Title:
		m_titleText->Draw(sr::Engine::Get().GetRenderer(), 850.0f, 490.0f);
		break;
	case SpaceGame::GameState::StartGame:
		break;
	case SpaceGame::GameState::StartLevel:
	case SpaceGame::GameState::Game:
	case SpaceGame::GameState::Dead:
		m_scoreText->Create(sr::Engine::Get().GetRenderer(), "Score: " + std::to_string(m_score), { 1.0f,1.0f,1.0f });
		m_livesText->Create(sr::Engine::Get().GetRenderer(), "Lives: " + std::to_string(m_lives), { 1.0f,1.0f,1.0f });

		m_scoreText->Draw(sr::Engine::Get().GetRenderer(), 30.0f, 30.0f);
		m_livesText->Draw(sr::Engine::Get().GetRenderer(), sr::Engine::Get().GetScreen().x - 120.0f, 30.0f);
		break;
	case SpaceGame::GameState::GameOver:
		m_gameOverText->Draw(sr::Engine::Get().GetRenderer(), 850.0f, 490.0f);
		break;
	default:
		break;
	}
	
	Game::Draw(renderer);
}

void SpaceGame::OnPlayerDead()
{
	m_lives--;
	m_gameState = (m_lives == 0) ? GameState::GameOver : GameState::StartLevel;
}

void SpaceGame::SpawnPlayer()
{
	auto player = sr::Factory::Instance().Create<Player>("Proto_Player");
	player->SetPosition(sr::Vector2{ (float)(sr::Engine::Get().GetScreen().x / 2), (float)(sr::Engine::Get().GetScreen().y / 2) });

	m_scene->AddActor(std::move(player));
}

void SpaceGame::SpawnEnemy()
{

	auto enemy = sr::Factory::Instance().Create<Enemy>("Proto_Enemy");
	enemy->SetPosition(sr::Vector2{ sr::RandomFloat(sr::Engine::Get().GetScreen().x), sr::RandomFloat(sr::Engine::Get().GetScreen().y) });
	enemy->SetSpeed(sr::RandomFloat(200.0f, 300.0f));

	m_scene->AddActor(std::move(enemy));
}
