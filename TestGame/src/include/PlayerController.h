#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <RSE.h>
#include <iostream>
#include <chrono>

#include "Projectile.h"

using namespace Advres::RSE;

class PlayerController : public Behaviour
{
public:
	bool tilePicker = false;
	int tileLayer = 0;
	Vector2f currentTileID = Vector2f(0, 0);
	TilemapComponent* tilemap;
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
	Vector2f input;
	int speed = 15;
	std::chrono::steady_clock::time_point sizeTp;
	Entity* uiImage;
	Vector2f norm;
	Rect debugRect;
	bool drawRect = true;
	Entity* dummy;
	TransformComponent* dummyTrans;
	Vector2f rayOrigin;
	bool rayToggeled = false;
	Rect test = { 300, 100, 32, 32 };

public:
	void HandleInput()
	{
		input.y = Input::GetActionAxis("moveUp");
		input.x = Input::GetActionAxis("moveLeft");
	}

	void Init() override
	{
		// dummy!!!
		dummy = RSECore::AddEntity();
		dummyTrans = dummy->AddComponent<TransformComponent>();
		Texture2D* tex = Resources::Load<Texture2D>("assets/mentl.jpg").get();
		auto dsp = dummy->AddComponent<Sprite>(tex);
		dsp->SetRenderRect({ 0, 0, tex->GetWidth(), tex->GetHeight() }, { 0, 0, 32, 32 });

		currentTileID = 0;
		input = { 0, 0 };
		transform = this->parent->GetComponent<TransformComponent>();
		sprite = this->parent->GetComponent<Sprite>();
		collider = this->parent->GetComponent<BoxCollider2D>();
		
		tilemap = RSECore::GetEntityByTag("tilemap")->GetComponent<TilemapComponent>();
		uiImage = RSECore::AddEntity();
		uiImage->tag = "uiImage";
		uiImage->AddComponent<TransformComponent>(Transform(RSECore::GetScreenWidth() - 32, RSECore::GetScreenHeight() - 32));
		auto temp = Resources::GetAllResources();
		uiImage->AddComponent<Sprite>(Resources::GetResource<Texture2D>(tilemap->GetTilesetTextureID()).get(), Transform());
		auto cp = uiImage->AddComponent<Sprite>(
			Resources::GetResource<Texture2D>(tilemap->GetTilesetTextureID()).get(),
			Transform());
		cp->SetRenderRect({ 0, 0, 16, 16 }, { 0, 0, 32, 32 });
		//cp->SetRenderRect(Vector2f(), Vector2f(16, 16), Vector2f(32, 32));

		std::cout << "Player Behvaiour Script initialized!" << std::endl;
	}

	Vector2f lastMousePos;
	void Update(float deltaTime) override
	{
		//std::cout << "some cool stuff idk" << std::endl;
		HandleInput();
		transform->velocity.x = speed * input.x * deltaTime * 10;
		transform->velocity.y = speed * input.y * deltaTime * 10;

		//Vector2f mouse = RSECore::GetMousePosition();
		//Vector2f dir = mouse - transform->position;
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
			fmt::println("Layer: {}", tileLayer);
		}
		if (Input::mouse.wheel.y != 0 && elapsed >= 30)
		{
			Vector2f new_scale = camera->GetScale() + (0.05f * Input::mouse.wheel.y);
			new_scale = Vector2f::Clamp(new_scale, { 0.3f, 0.3f }, { 1.3f, 1.3f });
			camera->SetScale(new_scale);
		}
		if (Input::mouse.button == MouseBtn::LMB && elapsed >= 30)
		{
			if (true)
			{
				if (rayToggeled)
				{
					Vector2f mouse = CameraModule::GetMousePositionInWorld();
					rayOrigin = mouse;
				}
				else if (!tilePicker)
				{
					Vector2f mouse = CameraModule::GetMousePositionInWorld();
					Vector2f vec;
					vec.x = Mathf::Round((int) mouse.x % gridSize);
					vec.y = Mathf::Round((int) mouse.y % gridSize);
					Vector2f snapped;
					snapped.x = Mathf::Round(mouse.x - vec.x);
					snapped.y = Mathf::Round(mouse.y - vec.y);
					tilemap->AddTile(snapped, currentTileID, tileLayer);
				}
				else
				{
					Vector2f mouse = RSECore::GetMousePosition();
					Vector2f pos_relative_to_trans = mouse - tileset->GetComponent<TransformComponent>()->position;
					Vector2f snapped = pos_relative_to_trans % gridSize;
					snapped = pos_relative_to_trans - snapped;
					currentTileID = snapped / 2;
				}
			}
			else
			{
				Vector2f mouse = CameraModule::GetMousePositionInWorld();
				dummyTrans->position = mouse;
			}
			sizeTp = now;
		}
		if (Input::mouse.button == MouseBtn::RMB && elapsed >= 30 && !tilePicker)
		{
			Vector2f mouse = CameraModule::GetMousePositionInWorld();
			Vector2f vec;
			vec.x = Mathf::Round((int) mouse.x % gridSize);
			vec.y = Mathf::Round((int) mouse.y % gridSize);
			Vector2f snapped;
			snapped.x = Mathf::Round(mouse.x - vec.x);
			snapped.y = Mathf::Round(mouse.y - vec.y);
			tilemap->RemoveTile(snapped, tileLayer);
			sizeTp = now;
		}
		if (Input::IsActionActive("tilePicker") && elapsed >= 200)
		{
			if (!tilePicker)
			{
				tilemap->parent->render = false;
				//tileset->render = true;
				tilePicker = true;
			}
			else
			{
				tilemap->parent->render = true;
				//tileset->render = false;
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
		// Mouse raycast
		if (Input::IsKeyDown(Key::F) && elapsed >= 200)
		{
			rayToggeled = (!rayToggeled) ? true : false;
			sizeTp = now;
		}
	}

	void BoxCollider2D_OnEnter(const ComponentCollideResult2D result) override
	{

	}

	void BoxCollider2D_OnExit(const ComponentCollideResult2D result) override
	{

	}

	void Render(float deltaTime) override
	{
		static TransformComponent* player_transform = parent->GetComponent<TransformComponent>();
		static SDL_Rect& player_collider = parent->GetComponent<BoxCollider2D>()->colliderRect;
		static Vector2f& player_velocity = parent->GetComponent<TransformComponent>()->velocity;

		// Collision testing
		Vector2f cn_point;
		Vector2f cn_normal;
		float cn_fraction;
		if (Collision::SweptAABB(player_collider, test, player_velocity, cn_point, cn_normal, cn_fraction, deltaTime))
		{
			fmt::println("{}", cn_normal);
			player_velocity *= -1;
			//RSECore::DrawDebugRect({ cn_point.x, cn_point.y, 3, 3 });
			//player_transform->position = cn_point;
		}

		// Tile drawing stuff
		RSECore::DrawRect(test);
		//SDL_RenderDrawPoint(RSECore::sdlRenderer, norm.x, norm.y);
		//map->Render();
		if (tilePicker)
		{
			auto trans = tileset->GetComponent<TransformComponent>();
			auto sp = parent->GetComponent<Sprite>()->texture;
			SDL_Rect src_r = { 0, 0, sp->GetWidth(), sp->GetHeight() };
			SDL_Rect dest_r = { (int) trans->position.x, (int) trans->position.y, sp->GetWidth() * (int) tileset->GetComponent<TransformComponent>()->scale.x, sp->GetHeight() * (int) tileset->GetComponent<TransformComponent>()->scale.y };
			RSECore::DrawTextureOnScreen(sp, &src_r, &dest_r);
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

		// Draw Mouse ray
		if (rayToggeled)
		{
			Vector2f mouse = CameraModule::GetMousePositionInWorld();
			RSECore::DrawLine(rayOrigin, mouse, 255, 0);
			BoxCollider2D* collider = this->parent->GetComponent<BoxCollider2D>();
			Vector2f cn_point;
			Vector2f cn_normal;
			float fraction;
			if (Collision::RayVsRect(rayOrigin, mouse, collider->colliderRect, cn_point, cn_normal, fraction))
			{
				Vector2f offset = { (cn_normal.x < 0) ? -16 : 0, (cn_normal.y < 0) ? -16 : 0 };
				RSECore::DrawRect({ (int) cn_point.x + (int) offset.x, (int) cn_point.y + (int) offset.y, 16, 16}, 255, 0, 0);
				collider->colour = { 255, 0, 0, 255 };
			}
			else collider->colour = { 255, 200, 0, 255 };
		}
	}
};

#endif // PLAYERCONTROLLER_H