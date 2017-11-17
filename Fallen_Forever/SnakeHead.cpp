#include "SnakeHead.h"
#include "Game.h"
#include "Messenger.h"
#include "Message.h"
#include "SnakeBody.h"
#include "GlobalDefines.h"
#include "GameThreadUnsafeScope.h"

SnakeHead::SnakeHead(Game* _Game)
{
	mGame = _Game;
	mSnakeHeadRadius = 20;
	mLayer = LAYER_GAME_BACKGROUND;
	AttachToMessenger(mGame->GetMessenger("KeyEvents"));
	mSnakeHeadRect = sf::CircleShape(mSnakeHeadRadius, 4);
	CreateCollider(&mPosition.first, &mPosition.second, &mSnakeHeadRadius);
	//mSnakeHeadRect.setRotation(45);
	mPosition.first = 100;
	mPosition.second = 100;
	mSnakeHeadRect.setPosition(mPosition.first, mPosition.second);
	mGame->AddObjectToRenderer(this, mLayer);
	mAddedToRenderer = true;
}

SnakeHead::~SnakeHead()
{
	if (mGame != nullptr && mAddedToRenderer)
	{
		GameThreadUnsafeScope ScopeLock(mGame);
		mGame->RemoveObjectFromRenderer(this, mLayer);
		mAddedToRenderer = false;
	}
}

void SnakeHead::RenderTick(sf::RenderWindow* _RenderWindow)
{
	mSnakeHeadRect.setPosition(mPosition.first, mPosition.second);
	_RenderWindow->draw(mSnakeHeadRect);

	//if (mSnakeBody != nullptr)
	//{
	//	mSnakeBody->RenderTick(_RenderWindow);
	//}
}

void SnakeHead::ReadMessage(Message* _Message)
{
	if (_Message->GetMessageType() == MESSAGE_TYPE_INPUT)
	{
		if (_Message->GetMessageDouble() == sf::Keyboard::W)
		{
			TurnUp();
		}
		else if (_Message->GetMessageDouble() == sf::Keyboard::S)
		{
			TurnDown();
		}
		else if (_Message->GetMessageDouble() == sf::Keyboard::A)
		{
			TurnLeft();
		}
		else if (_Message->GetMessageDouble() == sf::Keyboard::D)
		{
			TurnRight();
		}
		else if (_Message->GetMessageDouble() == sf::Keyboard::Space)
		{
			AddBody();
		}
	}
}

void SnakeHead::AddBody()
{
	if (mSnakeBody != nullptr)
	{
		mSnakeBody->AddBody();
	}
	else
	{
		mSnakeBody = std::make_shared<SnakeBody>(this, mGame);
		mChildren.push_back(mSnakeBody);
	}

}

void SnakeHead::TurnUp()
{
	if (mVelocity.second != 0)
	{
		return;
	}

	if (mSnakeBody != nullptr)
	{
		if (!mSnakeBody->AddTurn(mVelocity.first > 1 ? true : false))
		{
			return;
		}
	}

	mVelocity.first = 0;
	mVelocity.second = -100;
}

void SnakeHead::TurnDown()
{
	if (mVelocity.second != 0)
	{
		return;
	}

	if (mSnakeBody != nullptr)
	{
		if (!mSnakeBody->AddTurn(mVelocity.first < -1 ? true : false))
		{
			return;
		}
	}

	mVelocity.first = 0;
	mVelocity.second = 100;
}

void SnakeHead::TurnLeft()
{
	if (mVelocity.first != 0)
	{
		return;
	}

	if (mSnakeBody != nullptr)
	{
		if (!mSnakeBody->AddTurn(mVelocity.second < -1 ? true : false))
		{
			return;
		}
	}

	mVelocity.first = -100;
	mVelocity.second = 0;
}

void SnakeHead::TurnRight()
{
	if (mVelocity.first != 0)
	{
		return;
	}

	if (mSnakeBody != nullptr)
	{
		if (!mSnakeBody->AddTurn(mVelocity.second > 1 ? true : false))
		{
			return;
		}
	}

	mVelocity.first = 100;
	mVelocity.second = 0;
}

void SnakeHead::Tick(sf::Time _DeltaTime)
{
	GameObject::Tick(_DeltaTime);
}

void SnakeHead::Collide(Collidable* _Collidable)
{
	// If we hit a snake body
	if (dynamic_cast<SnakeBody*>(_Collidable) != nullptr)
	{
		mGame->QueueMessage("GameEvents", std::make_unique<Message>(MESSAGE_TYPE_DOUBLE, COLLISION_WITH_SELF));
	}
}