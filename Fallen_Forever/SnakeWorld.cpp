#include "SnakeWorld.h"
#include "Game.h"
#include "GameObject.h"
#include "Controller.h"
#include "Messenger.h"
#include "Message.h"
#include "SnakeHead.h"
#include "GameThreadUnsafeScope.h"

void SnakeWorld::Initialize()
{
	// Construct our world here.
	mWorldRoot = std::make_shared<SnakeHead>(mGame);
}

SnakeWorld::~SnakeWorld()
{
}

/// <summary> 
/// Calls RenderTick on graphics scenegraph.
/// </summary>
void SnakeWorld::RenderTick(sf::RenderWindow* _RenderWindow)
{
	// Implementation moved to Game
}

/// <summary> 
/// Ticks all objects in traditional scene graph.
/// </summary>
void SnakeWorld::Tick(sf::Time _DeltaTime)
{
	mWorldRoot.get()->Tick(_DeltaTime);
}

/// <summary> 
/// Checks controls and ticks all controllers.
/// </summary>
void SnakeWorld::ControllerTick(sf::Time _DeltaTime)
{
	CheckControls();
	mWorldRoot.get()->ControllerTick(_DeltaTime);
}

/// <summary> 
/// Checks only for controls expected by the world.
/// </summary>
void SnakeWorld::CheckControls(int _OverrideControl)
{
	if (_OverrideControl == sf::Keyboard::Escape)
	{
		mGame->QueueMessage("GlobalEvents", std::make_unique<Message>(MESSAGE_TYPE_QUIT, "Escape key pressed."));
	}
}

/// <summary> 
/// Interprets messages passed.
/// </summary>
void SnakeWorld::ReadMessage(Message* _Message)
{
	switch (_Message->GetMessageType())
	{
	case MESSAGE_TYPE_INVALID:
		return;
		break;
	case MESSAGE_TYPE_INPUT:
		CheckControls(_Message->GetMessageDouble());
		break;
	case MESSAGE_TYPE_DOUBLE:
		if (_Message->GetMessageDouble() == COLLISION_WITH_SELF)
		{
			mGame->QueueMessage("GlobalEvents", std::make_unique<Message>(MESSAGE_TYPE_DOUBLE, (double)RESTART_LEVEL));
		}
		break;
	case MESSAGE_TYPE_STRING:

		break;
	default:

		break;
	}
}