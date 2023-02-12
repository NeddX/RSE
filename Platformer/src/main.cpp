#include <RSE.h>
#include <sdafx.h>
#include <boost/filesystem.hpp>

using namespace Advres::RSE;

class Game : public RSECore
{
public:
	std::shared_ptr<Texture2D> swooshSheet, charSheet, tileSheet;
	Entity* player;

public:
	Game()
	{
		// Input
		Input::AddAction<KeyBind>("esc", { {KeyCode::ESCAPE} });
		
		// Load resources
		swooshSheet	= Resources::Load<Texture2D>("assets/swoosh.png");
		charSheet	= Resources::Load<Texture2D>("assets/characters.png");
		tileSheet	= Resources::Load<Texture2D>("assets/sheet.png");	
	}	

protected:
	void Start() override
	{		
		player = AddEntity();
		auto transform = player->AddComponent<TransformComponent>();
		auto sprite = player->AddComponent<Sprite>(tileSheet.get());
		// 9, 42 ; 15 x 20
		//sprite->SetRenderRect({ 9, 42, 15, 20 }, { 0, 0, 30, 40 });
	}
	void Update(float deltaTime) override
	{
		if (Input::IsActionActive("esc"))
			Dispose();
	}
	void Render(float deltaTime) override
	{
		
	}
};

int main(int argc, char* argv[])
{
	boost::filesystem::path path = argv[0];
	boost::filesystem::path parent = path.parent_path();
	boost::filesystem::current_path(parent);

	Game game;
	game.Init("Platformer game", 720, 600);
	
	return 0;
}
