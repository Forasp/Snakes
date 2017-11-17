#pragma once

#include "GameObject.h"
#include "Collidable.h"
#include "SFML/Graphics/CircleShape.hpp"
#include <queue>

class Game;
class Message;
class SnakeBody;

class SnakeHead : public GameObject, public Collidable
{
public:
	SnakeHead(Game* _Game);
	~SnakeHead();
	void RenderTick(sf::RenderWindow* _RenderWindow);
	void ReadMessage(Message* _Message);
	void Tick(sf::Time _DeltaTime);
	void Collide(Collidable* _Collidable);
protected:

	void AddBody(void);

	void TurnUp(void);
	void TurnDown(void);
	void TurnLeft(void);
	void TurnRight(void);

	std::shared_ptr<SnakeBody> mSnakeBody;
	sf::CircleShape mSnakeHeadRect;
	Game* mGame;
	double mSnakeHeadRadius;
};