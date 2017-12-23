#pragma once
#include "GameObject.h"
#include "Collidable.h"
#include "SFML/Graphics/CircleShape.hpp"
#include <memory>

class SnakeHead;
class Game;

class SnakeBody : public GameObject, public Collidable
{
public:
	SnakeBody(Game* _Game, GameObject* _Parent = nullptr) : GameObject(_Game, _Parent) { InitializeGameObject(); }
	~SnakeBody();
	void AddBody();
	bool AddTurn(bool Left);
	void Tick(sf::Time _DeltaTime);
	void RenderTick(sf::RenderWindow* _RenderWindow);
	void Collide(Collidable* _Collidable);
	void HandleMessage(Message* _Message) {};
	void InitializeGameObject();
	void ReadMessage(Message* _Message) { GameObject::ReadMessage(_Message); Collidable::ReadMessage(_Message); }
	virtual void CollisionBroadcast();

protected:
	void ExecuteTurn();

	GameObject* mSnakeHead;
	sf::CircleShape mSnakeBodyRect;
	std::pair<double, double> mPreviousPosition;
	std::shared_ptr<SnakeBody> mSnakeBodyTail;
	bool mTurnLeft;
	bool mTurning;
	bool mFirstLink;
	double mTargetPosition;
	bool mTargetGreater;
	double mSnakeBodyRadius;
};