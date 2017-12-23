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
	SnakeHead(Game* _Game, GameObject* _Parent = nullptr) : GameObject(_Game, _Parent) { InitializeGameObject(); }
	~SnakeHead();
	void RenderTick(sf::RenderWindow* _RenderWindow);
	void Tick(sf::Time _DeltaTime);
	void Collide(Collidable* _Collidable);
	void HandleMessage(Message* _Message);
	void ReadMessage(Message* _Message) { GameObject::ReadMessage(_Message); Collidable::ReadMessage(_Message); }
	void InitializeGameObject();
	virtual void CollisionBroadcast();

protected:

	void AddBody(void);

	void TurnUp(void);
	void TurnDown(void);
	void TurnLeft(void);
	void TurnRight(void);

	std::shared_ptr<SnakeBody> mSnakeBody;
	sf::CircleShape mSnakeHeadRect;
	double mSnakeHeadRadius;
};