#include "SnakeBody.h"
#include "SnakeHead.h"
#include "Game.h"
#include <math.h>
#include "GlobalDefines.h"
#include "Message.h"
#include "GameThreadUnsafeScope.h"

SnakeBody::SnakeBody(GameObject* _SnakeHead, Game* _Game)
{
	mSnakeHead = _SnakeHead;
	mGame = _Game;
	mSnakeBodyTail = NULL;
	mSnakeBodyRadius = 20;
	mLayer = LAYER_GAME_BACKGROUND;
	mTurning = false;
	mFirstLink = true;
	mPosition = mSnakeHead->GetPosition();
	mVelocity = mSnakeHead->GetVelocity();

	if (mVelocity.first > 1)
	{
		mPosition.first -= mSnakeBodyRadius*2;
	}
	else if (mVelocity.first < -1)
	{
		mPosition.first += mSnakeBodyRadius*2;
	}
	else if (mVelocity.second > 1)
	{
		mPosition.second -= mSnakeBodyRadius*2;
	}
	else if (mVelocity.second < -1)
	{
		mPosition.second += mSnakeBodyRadius*2;
	}

	mSnakeBodyRect = sf::CircleShape(mSnakeBodyRadius, 4);
	mSnakeBodyRadius = mSnakeBodyRadius * 0.9;
	CreateCollider(&mPosition.first, &mPosition.second, &mSnakeBodyRadius);
	mSnakeBodyRect.setPosition(mPosition.first, mPosition.second);
	mGame->AddObjectToRenderer(this, mLayer);
	mAddedToRenderer = true;
};

SnakeBody::~SnakeBody()
{
	if (mGame != nullptr && mAddedToRenderer)
	{
		GameThreadUnsafeScope ScopeLock(mGame);
		mGame->RemoveObjectFromRenderer(this, mLayer);
		mAddedToRenderer = false;
	}
}

void SnakeBody::AddBody()
{
	if (mSnakeBodyTail != nullptr)
	{
		mSnakeBodyTail->AddBody();
	}
	else
	{
		mSnakeBodyTail = std::make_shared<SnakeBody>(this, mGame);

		mChildren.push_back(mSnakeBodyTail);
	}
}

void SnakeBody::RenderTick(sf::RenderWindow* _RenderWindow)
{
	mSnakeBodyRect.setPosition(mPosition.first, mPosition.second);
	_RenderWindow->draw(mSnakeBodyRect);

	//if (mSnakeBodyTail != nullptr)
	//{
	//	mSnakeBodyTail->RenderTick(_RenderWindow);
	//}
}

bool SnakeBody::AddTurn(bool Left)
{
	if (mTurning)
	{
		return false;
	}

	if (mVelocity.first != 0)
	{
		mTargetPosition = mSnakeHead->GetPosition().first;
		mTargetGreater = mTargetPosition > mPosition.first;
	}
	else if (mVelocity.second != 0)
	{
		mTargetPosition = mSnakeHead->GetPosition().second;
		mTargetGreater = mTargetPosition > mPosition.second;
	}
	
	mTurnLeft = Left;
	mPreviousPosition = mPosition;
	mTurning = true;

	return true;
}

void SnakeBody::Tick(sf::Time _DeltaTime)
{
	GameObject::Tick(_DeltaTime);

	if (mTurning)
	{
		if (mVelocity.first != 0)
		{
			if ((mTargetGreater && mPosition.first >= mTargetPosition) || 
				(!mTargetGreater && mPosition.first <= mTargetPosition))
			{
				ExecuteTurn();
			}
		}
		else if (mVelocity.second != 0)
		{
			if ((mTargetGreater && mPosition.second >= mTargetPosition) ||
				(!mTargetGreater && mPosition.second <= mTargetPosition))
			{
				ExecuteTurn();
			}
		}
	}
}

void SnakeBody::ExecuteTurn()
{
	// If we're going right.
	if (mVelocity.first > 1)
	{
		mPosition.second = mTurnLeft ? mPosition.second - abs(mPosition.first - mSnakeHead->GetPosition().first) :
										mPosition.second + abs(mPosition.first - mSnakeHead->GetPosition().first);
		mPosition.first = mSnakeHead->GetPosition().first;
		mVelocity.first = 0;
		mVelocity.second = mTurnLeft ? -100 : 100;
	}
	// If we're going left.
	else if (mVelocity.first < -1)
	{
		mPosition.second = mTurnLeft ? mPosition.second + abs(mPosition.first - mSnakeHead->GetPosition().first) :
										mPosition.second - abs(mPosition.first - mSnakeHead->GetPosition().first);
		mPosition.first = mSnakeHead->GetPosition().first;
		mVelocity.first = 0;
		mVelocity.second = mTurnLeft ? 100 : -100;
	}
	// If we're going down.
	else if (mVelocity.second > 1)
	{
		mPosition.first = mTurnLeft ? mPosition.first + abs(mPosition.second - mSnakeHead->GetPosition().second) :
										mPosition.first - abs(mPosition.second - mSnakeHead->GetPosition().second);
		mPosition.second = mSnakeHead->GetPosition().second;
		mVelocity.first = mTurnLeft ? 100 : -100;
		mVelocity.second = 0;
	}
	// If we're going up.
	else if (mVelocity.second < -1)
	{
		mPosition.first = mTurnLeft ? mPosition.first - abs(mPosition.second - mSnakeHead->GetPosition().second) :
										mPosition.first + abs(mPosition.second - mSnakeHead->GetPosition().second);
		mPosition.second = mSnakeHead->GetPosition().second;
		mVelocity.first = mTurnLeft ? -100 : 100;
		mVelocity.second = 0;
	}

	if (mSnakeBodyTail)
	{
		mSnakeBodyTail->AddTurn(mTurnLeft);
	}

	mTurning = false;
}

void SnakeBody::Collide(Collidable* _Collidable)
{

}