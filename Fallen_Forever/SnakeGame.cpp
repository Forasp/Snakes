#include "SnakeGame.h"
#include "SnakeWorld.h"
#include "Messenger.h"
#include "Message.h"
#include <memory>

std::shared_ptr<World> SnakeGame::GetNewWorld()
{
	return std::make_shared<SnakeWorld>(this);
}

void SnakeGame::InitializeGame(sf::RenderWindow* _RenderWindow)
{
	Game::InitializeGame(_RenderWindow);

	AttachToMessenger(GetMessenger("KeyEvents").get());
	AttachToMessenger(GetMessenger("GlobalEvents").get());
}