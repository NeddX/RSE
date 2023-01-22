#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <RSE.h>
#include <iostream>
#include <chrono>

#include "Projectile.h"

using namespace RSE;

class PlayerController : public Behaviour
{
public:
	bool tilePicker = false;
	int tileLayer = 0;
	Vector2 currentTileID = Vector2(0, 0);
	TileMapComponent* tilemap;
	Entity* tileset;
	std::unordered_map<std::string, std::shared_ptr<Texture2D>>* textureResources;
	bool drawGrid = false;
	int gridSize = 16;
	Camera2DComponent* camera;

private:
	TransformComponent* transform;
	TransformComponent* projTransComp;
	Sprite* sprite;
	BoxCollider2D* collider;
	Vector2 input;
	int speed = 20;
	std::chrono::steady_clock::time_point sizeTp;
	Entity* uiImage;
	Vector2 norm;

public:
	void HandleInput()
	{
		input.y = Input::GetActionAxis("moveUp");
		input.x = Input::GetActionAxis("moveLeft");
	}

	void Init() override
	{
		currentTileID = 0;
		input = { 0, 0 };
		transform = this->entity->GetComponent<TransformComponent>();
		sprite = this->entity->GetComponent<Sprite>();
		collider = this->entity->GetComponent<BoxCollider2D>();
		
		tilemap = RSECore::GetEntityByTag("tilemap")->GetComponent<TileMapComponent>();
		uiImage = RSECore::AddEntity();
		uiImage->AddComponent<TransformComponent>(Transform(RSECore::GetScreenWidth() - 32, RSECore::GetScreenHeight() - 32));
		auto temp = Resources::GetAllResources();
		uiImage->AddComponent<Sprite>(Resources::GetResource<Texture2D>(tilemap->GetTilesetTextureID()).get(), Transform());
		auto cp = uiImage->AddComponent<Sprite>(
			Resources::GetResource<Texture2D>(tilemap->GetTilesetTextureID()).get(),
			Transform());
		cp->SetRenderRect({ 0, 0, 16, 16 }, { 0, 0, 32, 32 });
		//cp->SetRenderRect(Vector2(), Vector2(16, 16), Vector2(32, 32));

		std::cout << "Player Behvaiour Script initialized!" << std::endl;
	}

	Vector2 lastMousePos;
	void Update(float deltaTime) override
	{
		//std::cout << "some cool stuff idk" << std::endl;
		HandleInput();
		transform->velocity.x = speed * input.x * deltaTime * 10;
		transform->velocity.y = speed * input.y * deltaTime * 10;

		//Vector2 mouse = RSECore::GetMousePos();
		//Vector2 dir = mouse - transform->position;
		//norm = dir.Normalize();
		//std::cout << "Unit vec dist: " << norm << "       \r";

		// scrap the rest for now
		//return;

		auto now = std::chrono::steady_clock::now();
		float elapsed = (float) std::chrono::duration_cast<std::chrono::milliseconds>(now - sizeTp).count();
		float axis = Input::GetActionAxis("changeLayer");
		if (axis != 0 && elapsed >= 100)
		{
			tileLayer += axis;
			sizeTp = now;
			std::cout << "Layer: " << tileLayer << std::endl;
		}
		if (Input::mouse.wheel.y != 0 && elapsed >= 30)
		{
			Vector2 newScale;
			newScale.x = camera->GetScale().x + (0.05f * Input::mouse.wheel.y);
			newScale.y = camera->GetScale().y + (0.05f * Input::mouse.wheel.y);
			camera->SetScale(newScale);
			//Vector2 newScale = ssc - 0.05f * Input::mouse.wheel.y;
			//camera->SetScale(newScale);
			//if (camera->GetScale().x > 1.3f) camera->SetScale({ 1.3f, 1.3f });
			//else if (camera->GetScale().x < 0.5f) camera->SetScale({0.5f, 0.5f});

			/*camera->SetScale(camera->GetScale() - 0.05f * Input::mouse.wheel.y);
			if (camera->GetScale() > 1.3f) camera->SetScale(1.3f);
			else if (camera->scale < 0.5f) camera->scale = 0.5f;
			camera->m_Viewport.w = camera->GetSize().x * camera->scale;
			camera->m_Viewport.h = camera->GetSize().y * camera->scale;
			camera->transform.position = Vector2(-(camera->m_Viewport.w / 2), -(camera->m_Viewport.h / 2));*/
		}
		if (Input::mouse.button == MouseBtn::LMB && elapsed >= 30)
		{
			if (!tilePicker)
			{
				Vector2 mouse = RSECore::GetMousePos();
				Vector2 vec;
				vec.x = (int) mouse.x % gridSize;
				vec.y = (int) mouse.y % gridSize;
				Vector2 snapped;
				snapped.x = Mathf::Round(mouse.x - vec.x);
				snapped.y = Mathf::Round(mouse.y - vec.y);
				tilemap->AddTile(snapped, currentTileID, tileLayer);
			}
			else
			{
				Vector2 mouse = RSECore::GetMousePos();
				Vector2 pRelative2T = mouse - tileset->GetComponent<TransformComponent>()->position;
				Vector2 snapped = pRelative2T % gridSize;
				snapped = pRelative2T - snapped;
				currentTileID = snapped / 2;
			}
			sizeTp = now;
		}
		if (Input::mouse.button == MouseBtn::RMB && elapsed >= 30 && !tilePicker)
		{
			Vector2 mouse = RSECore::GetMousePos();
			Vector2 vec;
			vec.x = (int) mouse.x % gridSize;
			vec.y = (int) mouse.y % gridSize;
			Vector2 snapped;
			snapped.x = Mathf::Round(mouse.x - vec.x);
			snapped.y = Mathf::Round(mouse.y - vec.y);
			tilemap->RemoveTile(snapped, tileLayer);
			sizeTp = now;
		}
		if (Input::IsActionActive("tilePicker") && elapsed >= 200)
		{
			if (!tilePicker)
			{
				tilemap->render = false;
				tileset->render = true;
				tilePicker = true;
			}
			else
			{
				tilemap->render = true;
				tileset->render = false;
				tilePicker = false;
			}
			sizeTp = now;
		}
		if (Input::IsActionActive("toggleGrid") && elapsed >= 200)
		{
			if (drawGrid) drawGrid = false;
			else drawGrid = true;
			sizeTp = now;
		}
		// Middle between the mouse and the player
		/*Vector2 mousePos = RSECore::GetMousePos();
		float distance = transform->position.DistanceTo(mousePos);
		if (distance > 200 && transform->velocity.Equals({ 0, 0 }))
		{
			if (distance < 400)
			{
				Vector2 lerped = transform->position.Lerp(RSECore::GetMousePos(), 0.4);
				debugRect->GetComponent<TransformComponent>()->position = lerped;
				lastMousePos = mousePos;
			}
			else
			{
				Vector2 vec = RSECore::GetMousePos().Clamp({ lastMousePos.x, lastMousePos.y });
				Vector2 lerped = transform->position.Lerp(vec, 0.4);
				debugRect->GetComponent<TransformComponent>()->position = lerped;
				std::cout << "lerp: " << vec << std::endl;
			}
		}
		else
		{
			debugRect->GetComponent<TransformComponent>()->position = transform->position;
		}*/
	}

	void BoxCollider2D_OnEnter(const ComponentCollideResult2D result) override
	{
	}

	void BoxCollider2D_OnExit(const ComponentCollideResult2D result) override
	{
	}

	void Render(float deltaTime) override
	{
		//SDL_RenderDrawPoint(RSECore::sdlRenderer, norm.x, norm.y);
		//map->Render();
		if (tilePicker)
		{
			auto trans = tileset->GetComponent<TransformComponent>();
			int width = tileset->GetComponent<Sprite>()->texture->GetWidth() * tileset->GetComponent<TransformComponent>()->scale.x;
			int height = tileset->GetComponent<Sprite>()->texture->GetHeight() * tileset->GetComponent<TransformComponent>()->scale.y;
			int columns = width / gridSize;
			int rows = height / gridSize;
			SDL_SetRenderDrawColor(RSECore::sdlRenderer, 150, 150, 150, 0);
			for (int x = 0; x < rows + 1; ++x)
			{
				SDL_RenderDrawLine(
					RSECore::sdlRenderer, 
					trans->position.x, 
					trans->position.y + (x * gridSize), 
					trans->position.x + width, 
					trans->position.y + (x * gridSize));
			}
			for (int y = 0; y < columns; ++y)
			{
				SDL_RenderDrawLine(
					RSECore::sdlRenderer,
					trans->position.x + (y * gridSize),
					trans->position.y,
					trans->position.x + (y * gridSize),
					trans->position.y + height
				);
			}
		}
		else if (!tilePicker && drawGrid)
		{
			int columns = RSECore::GetScreenWidth() / gridSize;
			int rows = RSECore::GetScreenHeight() / gridSize;

			SDL_SetRenderDrawColor(RSECore::sdlRenderer, 150, 150, 150, 10);
			for (int y = 0; y < columns; ++y)
			{
				SDL_RenderDrawLine(RSECore::sdlRenderer, 0, y * gridSize, RSECore::GetScreenWidth(), y * gridSize);
			}

			for (int x = 0; x < columns; ++x)
			{
				SDL_RenderDrawLine(RSECore::sdlRenderer, x * gridSize, 0, x * gridSize, RSECore::GetScreenHeight());
			}
		}
	}
};

#endif // PLAYERCONTROLLER_H