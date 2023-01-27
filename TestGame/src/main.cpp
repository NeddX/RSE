#include <iostream>
#include <format>
#include <memory>
#include <RSE.h>
#include <fstream>
#include <string>

#include <sdafx.h>

#ifdef _WIN32
#include "windows.h"
#define symLoad GetProcAddress
#else
#include "dlfcn.h"
#define symLoad dlsym
#endif

#include "include/PlayerController.h"
#include "include/EnemyAI.h"

#define WIDTH	800 
#define HEIGHT	600
#define FPS		300

using namespace Advres::RSE;

enum Groups
{
	World,
	Player,
	Enemy,
	colliders,
	UI
};

class Test : public RSECore
{
public:
	int gridSize = 32;
	Entity* player;
	Entity* enemy;
	Entity* dummy;
	Entity* camObj;
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> textureResources;
	PlayerController* controller;
	Camera2DComponent* camera;

public:
	void Start() override
	{
		// Call Native C# functions from C++, crazy stuff!
		/*
#ifdef _WIN32
		HINSTANCE handle = LoadLibrary("CSInvoke.dll");
#else
		void* handle = dlopen(path, RTLD_LAZY);
#endif
		typedef  int* (*myFunc)(int*);
		myFunc import = (myFunc) symLoad(handle, "ReturnMe");
		char str1[] = "DA STR!";
		std::cout << "CSInvoke.Str(): " << (char*) import((int*) &str1) << std::endl;*/

		// Configure input
		Input::AddAction<AxisBind>("moveUp",	  { Key::S, Key::W });
		Input::AddAction<AxisBind>("moveLeft",    { Key::D, Key::A });
		Input::AddAction<AxisBind>("changeLayer", { Key::ARROW_UP,Key::ARROW_DOWN });
		Input::AddAction<KeyBind>("tilePicker",   { {Key::TAB} });
		Input::AddAction<KeyBind>("toggleGrid",   { {Key::G} });

		textureResources["dunTileset"] = Resources::Load<Texture2D>("Base.rse/Environment/Levels/tileset2.png");

		TilemapComponent* tmap;

		Entity* tilemap;
		if (true)
		{
			const char path[] = "Base.rse/Environment/Levels/mgr.rym";
			Serializer::Deserialize(path, entityManager.get());
			tilemap = GetEntityByTag("tilemap");
			tmap = tilemap->GetComponent<TilemapComponent>();
			tilemap->AddComponent<TilemapCollider2D>(1);
			printf("Total tile count: %zu\n", tmap->GetTotalSize());
		}
		else
		{
			tilemap = AddEntity();
			tilemap->tag = "tilemap";
			tmap = tilemap->AddComponent<TilemapComponent>(textureResources["dunTileset"].get(), Vector2(16, 16), Vector2(32, 32));
			tilemap->SetGroup(Groups::World);
		}

		Entity* tilePicker = AddEntity();
		tilePicker->tag = "tilePicker";
		//tilePicker->SetGroup(Groups::World);
		player = AddEntity();
		player->SetGroup(Groups::Player);
		camObj = AddEntity();
		//enemy = AddEntity();

		auto furnace = AddEntity();
		furnace->tag = "furnace";
		furnace->AddComponent<TransformComponent>(Transform(Vector2(500, 350)));
		auto fsp = furnace->AddComponent<Sprite>(textureResources["dunTileset"].get(), Transform());
		auto spa = furnace->AddComponent<SpriteSheetAnimation>(0, 7, 50, AnimPlaybackMode::PingPong);
		spa->Play();
		fsp->SetRenderRect({ 0, 80, 16, 16 }, { 0, 0, 32, 32 });
		furnace->SetGroup(Groups::World);

		auto trans = tilePicker->AddComponent<TransformComponent>(Transform());
		auto sp = tilePicker->AddComponent<Sprite>(textureResources["dunTileset"].get(), Transform());
		tilePicker->render = false;
		trans->scale = { 2, 2 };
		trans->position = Vector2(WIDTH / 2 - (textureResources["dunTileset"]->GetWidth() * trans->scale.x / 2), HEIGHT / 2 - (textureResources["dunTileset"]->GetHeight() * trans->scale.y / 2));
		Vector2 vec;
		vec.x = (int) trans->position.x % gridSize;
		vec.y = (int) trans->position.y % gridSize;
		trans->position.x = (int) trans->position.x - vec.x;
		trans->position.y = (int) trans->position.y - vec.y;

		player->tag = "player";
		auto ptrans = player->AddComponent<TransformComponent>(Transform(Vector2(600, 350)));
		auto ps = player->AddComponent<Sprite>(textureResources["dunTileset"].get(), Transform());
		ps->SetRenderRect({ 368, 112, 16, 16 }, { 0, 0, 32, 32 });
		auto sheetanim = player->AddComponent<SpriteSheetAnimation>(0, 5, 200, AnimPlaybackMode::PingPong);
		//sheetanim->Play();

		// Camera
		camObj->AddComponent<TransformComponent>(Transform(WIDTH, HEIGHT));
		camera = camObj->AddComponent<Camera2DComponent>(Transform(Vector2 { -(WIDTH / 2) + 32, -(HEIGHT / 2) + 32 }));
		camObj->tag = "kamera";
		
		// Behaviour code
		controller = player->AddComponent<PlayerController>();
		controller->camera = camera;
		controller->tilemap = tmap;
		controller->tileset = tilePicker;
		controller->gridSize = tmap->GetGridSize().x * trans->scale.x;
		controller->textureResources = &textureResources;

		// Collider
		auto playerCollider = player->AddComponent<BoxCollider2D>("player", Transform(), Vector2(32, 32));
		playerCollider->trigger = true;
		playerCollider->SetBehaviourObject(controller);
		playerCollider->OnCollide = &Behaviour::BoxCollider2D_OnCollide;
		playerCollider->OnEnter =	&Behaviour::BoxCollider2D_OnEnter;
		playerCollider->OnExit	=	&Behaviour::BoxCollider2D_OnExit;



		//enemy->tag = "Cultist";
		//enemy->AddComponent<TransformComponent>(Transform(Vector2(100, 200)));
		//auto es = enemy->AddComponent<Sprite>(textureResources["dunTileset"], Transform());
		//es->SetRenderRect(Vector2(176, 192), Vector2(32, 32), Vector2(64, 64));
		//EnemyAI* enemyController = enemy->AddComponent<EnemyAI>();
		//auto enemyCollider = enemy->AddComponent<BoxCollider2D>("enemy", Transform(), Vector2(32, 32));
		//enemyCollider->trigger = true;
	}

	void Update(float deltaTime) override
	{
		std::string fmt = fmt::format("FPS: {} Frames: {} DeltaTime: {}", GetFrameRate(), GetFrameCount(), deltaTime);
		SetWindowTitle(fmt::format("FPS: {} Frames: {} Delta: {}", GetFrameRate(), GetFrameCount(), deltaTime).c_str());

		static TransformComponent* camTrans = camObj->GetComponent<TransformComponent>();
		static Camera2DComponent* camera = camObj->GetComponent<Camera2DComponent>();
		static TransformComponent* playerTrans = player->GetComponent<TransformComponent>();
		static float camSpeed = 4.0f;
		camTrans->position = Vector2::Lerp(camTrans->position, playerTrans->position, deltaTime * camSpeed);
		if (Input::IsKeyDown(Key::E))
		{
			std::cout << "Serialization started." << std::endl;
			Serializer::SerializeScene("actormgr.rym", entityManager.get());
			std::cout << "Serialization ended." << std::endl;
		}
		//static bool didSerialize = false;
		//if (Input::IsActionActive("Serialize"))
		//{
		//	std::cout << "Serialization started." << std::endl;
		//	Serializer::SerializeScene("actormgr.rym", entityManager.get());
		//	didSerialize = true;
		//	std::cout << "Serialization ended." << std::endl;
		//}
		//if (Input::IsActionActive("Deserialize"))
		//{
		//	entityManager.reset();
		//	entityManager = std::make_shared<EntityManager>();
		//	
		//	std::cout << "Loading actormgr.rse..." << std::endl;
		//	Serializer::Deserialize("actormgr.rse", entityManager.get());
		//	std::cout << "Deserialization done." << std::endl;
		//}
	}

	void Render(float deltaTime) override
	{
		//map->Render();
	}
};

int main(int argc, char* argv[])
{
	Test* test = new Test();
	try
	{
		RSEStatus statusCode = test->Init("RSE - Framework", WIDTH, HEIGHT, FPS);
		if (statusCode != RSEStatus::RSE_OK)
		{
			std::cout << "ERROR: Something went wrong! Status Code: " << (int) statusCode << std::endl;
		}
		delete test;
	}
	catch (const RException& ex)
	{
		// WIN32 EXCL
		//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		//SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		std::cout << ex.Message() << std::endl;
		return ex.ErrorCode();
		//etConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	return 0;
}