#include "Assets.h"

namespace assets {
	sr::Mesh sharedMesh = { {sr::Vector2 {0,1}, sr::Vector2{0,-1}, sr::Vector2 {-1.5f,-2}, sr::Vector2{-0.5f,0}, sr::Vector2 {-1.5f,2}, sr::Vector2 {0,1}}, sr::Color{1.0f,1.0f,1.0f} };
	
	std::vector<sr::Mesh> playerMeshes = { sharedMesh, sr::Mesh{ {sr::Vector2{1.5f, 0}, sr::Vector2 {0,1}, sr::Vector2{0,-1}, sr::Vector2{1.5f, 0}}, sr::Color{0.0f,1.0f,1.0f}} };
	std::vector<sr::Mesh> enemyMeshes = { sharedMesh, sr::Mesh{ {sr::Vector2{1.5f, 0}, sr::Vector2 {0,1}, sr::Vector2{0,-1}, sr::Vector2{1.5f, 0}}, sr::Color{1.0f,0.0f,0.0f}} };
	
	sr::Mesh bulletMesh{
	  {
		sr::Vector2{ -1, -1 },
		sr::Vector2{ 1, 0 },
		sr::Vector2{ -1, 1 },
		sr::Vector2{ -1, -1 },
	  },
	  sr::Color{ 1.0f, 0.0f, 0.0f }
	};

	std::shared_ptr<sr::Model> playerModel = std::make_shared<sr::Model>(playerMeshes);
	std::shared_ptr<sr::Model> enemyModel = std::make_shared<sr::Model> (enemyMeshes);

	std::shared_ptr<sr::Model> bulletModel = std::make_shared<sr::Model>(std::vector{ bulletMesh });
}