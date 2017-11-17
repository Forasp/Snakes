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
	SnakeBody(GameObject* _SnakeHead, Game* _Game);
	~SnakeBody();
	void AddBody();
	bool AddTurn(bool Left);
	void Tick(sf::Time _DeltaTime);
	void RenderTick(sf::RenderWindow* _RenderWindow);
	void Collide(Collidable* _Collidable);

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