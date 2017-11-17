#include "World.h"
#include "Game.h"
#include "GameObject.h"
#include "Controller.h"
#include "Messenger.h"
#include "Message.h"
#include "SnakeHead.h"

World::World(Game* _Game)
{
	mGame = _Game;

	AttachToMessenger(mGame->GetMessenger("KeyEvents"));
	AttachToMessenger(mGame->GetMessenger("GameEvents"));
}

World::~World()
{

}

/// <summary> 
/// Calls RenderTick on graphics scenegraph.
/// </summary>
void World::RenderTick(sf::RenderWindow* _RenderWindow)
{
	// Implementation moved to Game
}

/// <summary> 
/// Ticks all objects in traditional scene graph.
/// </summary>
void World::Tick(sf::Time _DeltaTime)
{
	mWorldRoot.get()->Tick(_DeltaTime);
}

/// <summary> 
/// Checks controls and ticks all controllers.
/// </summary>
void World::ControllerTick(sf::Time _DeltaTime)
{
	CheckControls();
	mWorldRoot.get()->ControllerTick(_DeltaTime);
}

/// <summary> 
/// Checks only for controls expected by the world.
/// </summary>
void World::CheckControls(int _OverrideControl)
{
	if (_OverrideControl == sf::Keyboard::Escape)
	{
		mGame->QueueMessage("GlobalEvents", std::make_unique<Message>(MESSAGE_TYPE_QUIT, "Escape key pressed."));
	}
}

/// <summary> 
/// Interprets messages passed.
/// </summary>
void World::ReadMessage(Message* _Message)
{
	 
}