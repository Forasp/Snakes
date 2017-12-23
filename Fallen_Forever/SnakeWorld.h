#pragma once
#include "World.h"

class Message;

class SnakeWorld : public World
{
public:
	SnakeWorld(Game* _Game) : World(_Game)
	{
		mGame = _Game;
		Initialize();
	};

	~SnakeWorld();

	void Tick(sf::Time _DeltaTime);
	void RenderTick(sf::RenderWindow* _RenderWindow);
	void ControllerTick(sf::Time _DeltaTime);

	void ReadMessage(Message* _Message);

	std::shared_ptr<GameObject> GetWorldRoot() { return mWorldRoot; }
	std::shared_ptr<ResourceHolder> GetResourceHolder() { return mResourceHolder; }

protected:
	void Initialize();

	void CheckControls(int _OverrideControl = -1);

	std::shared_ptr<GameObject> mWorldRoot;

	std::shared_ptr<ResourceHolder> mResourceHolder;
};