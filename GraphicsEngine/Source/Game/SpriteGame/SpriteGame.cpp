#include "SpriteGame.h"
#include "Resources/ResourceManager.h"
#include "Engine.h"
#include "PlayerController.h"
#include "EnemyController.h"
#include "FlyingEnemyController.h"
#include "Renderer/Texture.h"


#include "Serialization/Json.h"

#include <memory>



bool SpriteGame::Initialize()
{
	Game::Initialize();

	sr::SetWorkingDirectory("SpriteGame");

	sr::SetSeed();

	m_scene = std::make_unique<sr::Scene>();
	m_scene->SetGame(this);
	m_scene->Load("Data/scene.json"); 

	

	sr::Engine::Get().GetAudio().AddSound("explosion", "Sounds/snd_badexplosion.wav");
	sr::Engine::Get().GetAudio().AddSound("shoot", "Sounds/snd_bomb.wav");
	sr::Engine::Get().GetAudio().AddSound("slash", "Sounds/snd_laz_c.wav");
	sr::Engine::Get().GetAudio().AddSound("hit", "Sounds/snd_damage.wav");

	m_menuFont = sr::Resources().GetWithID<sr::Font>("MenuFont","Fonts/gameFont.ttf", 64);
	m_gameFont = sr::Resources().GetWithID<sr::Font>("GameFont","Fonts/gameFont.ttf", 32);

	m_titleText = new sr::Text(m_menuFont);
	m_titleText->Create(sr::Engine::Get().GetRenderer(), "Survival", sr::Vector3{ 1.0f, 1.0f, 1.0f });
	m_gameOverText = new sr::Text(m_menuFont);
	m_gameOverText->Create(sr::Engine::Get().GetRenderer(), "Game Over", sr::Vector3{ 1.0f, 1.0f, 1.0f });

	m_scoreText = new sr::Text(m_gameFont);
	m_livesText = new sr::Text(m_gameFont);
	return true;
}

void SpriteGame::Update(float dt, float width, float height){
	switch (m_gameState)
	{
	case SpriteGame::GameState::Title: {
		if (sr::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) m_gameState = GameState::StartGame;
		break;
	}
	case SpriteGame::GameState::StartGame: {
		m_score = 0;
		m_lives = 3;
		m_gameState = GameState::StartLevel;
		break;
	}
	case SpriteGame::GameState::StartLevel: {
		m_stateTimer -= dt;
		if (m_stateTimer <= 0.0f) {
			m_scene->ClearActors();
			m_scene->Load("Data/level.json");
			SpawnPlayer();
			m_gameState = GameState::Game;
		}
		break;
	}
	case SpriteGame::GameState::Game:
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
	case SpriteGame::GameState::Dead:
		OnPlayerDead();
		m_stateTimer = 3.0f;
		break;
		
	case SpriteGame::GameState::GameOver:
		m_scene->ClearActors();
		if (sr::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) m_gameState = GameState::Title;
		break;
	default:
		break;
	}
	
	Game::Update(dt, width, height);
}

void SpriteGame::Draw(sr::Renderer& renderer){
	
	renderer.EnableCamera(false);
	renderer.DrawTexture(*sr::Resources().Get<sr::Texture>("Images/Dungeon_Background_1.png", sr::Engine::Get().GetRenderer()), sr::Transform(sr::Vector2{0.0f}, 0.0f, 5.0));
	renderer.EnableCamera();
	Game::Draw(renderer);
	renderer.EnableCamera(false);
	switch (m_gameState)
	{
	case SpriteGame::GameState::Title:
		m_titleText->Draw(sr::Engine::Get().GetRenderer(), 850.0f, 490.0f);
		break;
	case SpriteGame::GameState::StartGame:
		break;
	case SpriteGame::GameState::StartLevel:
	case SpriteGame::GameState::Game:
	case SpriteGame::GameState::Dead:
		m_scoreText->Create(sr::Engine::Get().GetRenderer(), "Score: " + std::to_string(m_score), { 1.0f,1.0f,1.0f });
		m_livesText->Create(sr::Engine::Get().GetRenderer(), "Lives: " + std::to_string(m_lives), { 1.0f,1.0f,1.0f });

		m_scoreText->Draw(sr::Engine::Get().GetRenderer(), 30.0f, 30.0f);
		m_livesText->Draw(sr::Engine::Get().GetRenderer(), sr::Engine::Get().GetScreen().x - 120.0f, 30.0f);
		break;
	case SpriteGame::GameState::GameOver:
		m_gameOverText->Draw(sr::Engine::Get().GetRenderer(), 850.0f, 490.0f);
		break;
	default:
		break;
	}
	renderer.EnableCamera();
	
}

void SpriteGame::OnPlayerDead()
{
	m_lives--;
	m_gameState = (m_lives == 0) ? GameState::GameOver : GameState::StartLevel;
}

void SpriteGame::SpawnPlayer()
{
auto player = sr::Factory::Instance().Create<PlayerController>("Proto_Player");
player->SetPosition(sr::Vector2{ 2160.0f, 2160.0f });

m_scene->AddActor(std::move(player));
}

void SpriteGame::SpawnEnemy()
{

	int enemyIndex = sr::RandomInt(2);
	if (enemyIndex == 0) {
		auto enemy = sr::Factory::Instance().Create<EnemyController>("Proto_Enemy");
		enemy->SetPosition(sr::Vector2{ sr::RandomFloat(sr::Engine::Get().GetScreen().x), sr::RandomFloat(sr::Engine::Get().GetScreen().y) });
		m_scene->AddActor(std::move(enemy));
	}
	else {
		auto enemy = sr::Factory::Instance().Create<FlyingEnemyController>("Proto_FlyingEnemy");
		enemy->SetPosition(sr::Vector2{ sr::RandomFloat(sr::Engine::Get().GetScreen().x), sr::RandomFloat(sr::Engine::Get().GetScreen().y) });
		m_scene->AddActor(std::move(enemy));
	}
	
}
