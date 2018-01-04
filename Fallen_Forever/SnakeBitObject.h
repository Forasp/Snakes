#pragma once

#include "GameObject.h"
#include "Collidable.h"
#include "SFML/Graphics/CircleShape.hpp"
#include <queue>

class Game;
class Message;
class SnakeBody;

class SnakeBitObject : public GameObject, public Collidable
{
public:
	SnakeBitObject(Game* _Game, GameObject* _Parent = nullptr) : GameObject(_Game, _Parent) { InitializeGameObject(); }
	~SnakeBitObject();
	void RenderTick(sf::RenderWindow* _RenderWindow);
	void Tick(sf::Time _DeltaTime);
	void Collide(Collidable* _Collidable);
	void HandleMessage(Message* _Message);
	void ReadMessage(Message* _Message) { GameObject::ReadMessage(_Message); Collidable::ReadMessage(_Message); }
	void InitializeGameObject();
	virtual void CollisionBroadcast() {};

protected:

	sf::CircleShape mRenderShape;
	double mBitRadius;
};