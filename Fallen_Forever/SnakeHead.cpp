#include "SnakeHead.h"
#include "Game.h"
#include "Messenger.h"
#include "Message.h"
#include "SnakeBody.h"
#include "GlobalDefines.h"
#include "GameThreadUnsafeScope.h"
#include "SnakeBitObject.h"

void SnakeHead::InitializeGameObject()
{
	mSnakeHeadRadius = 20;
	mRotation = 45;
	mLayer = LAYER_GAME_BACKGROUND;
	Listener::AttachToMessenger(mGame->GetMessenger("KeyEvents").get());
	mSnakeHeadRect = sf::CircleShape(mSnakeHeadRadius, 4);
	mSize = std::make_pair(mSnakeHeadRadius * 2, mSnakeHeadRadius * 2);
	CreateCollider(&mPosition, &mSize, &mRotation);
	//mSnakeHeadRect.setRotation(45);
	mPosition.first = 100;
	mPosition.second = 100;
	mSnakeHeadRect.setPosition(mPosition.first, mPosition.second);
	mGame->AddObjectToRenderer(this, mLayer);
	mAddedToRenderer = true;

	// TODO - Need to replace all the shape nonsense with images and scrap the following

	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			mCollisionAreaXS[i][j] = true;
		}
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			mCollisionAreaS[i][j] = true;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			mCollisionAreaM[i][j] = true;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mCollisionAreaL[i][j] = true;
		}
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			mCollisionAreaXL[i][j] = true;
		}
	}
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
	if (!_RenderWindow)
	{
		return;
	}

	mSnakeHeadRect.setPosition(mPosition.first, mPosition.second);
	_RenderWindow->draw(mSnakeHeadRect);

	//if (mSnakeBody != nullptr)
	//{
	//	mSnakeBody->RenderTick(_RenderWindow);
	//}
}

void SnakeHead::HandleMessage(Message* _Message)
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
		mSnakeBody = std::make_shared<SnakeBody>(mGame, this);
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
	if (mPosition.first < 0 + mSize.first / 2 ||
		mPosition.first > 1024 - mSize.first / 2 ||
		mPosition.second < 0 + mSize.second / 2 ||
		mPosition.second > 768 - mSize.second / 2)
	{
		mGame->QueueMessage("GameEvents", std::make_unique<Message>(MESSAGE_TYPE_DOUBLE, (double)COLLISION_WITH_SELF));
	}

	GameObject::Tick(_DeltaTime);
}

void SnakeHead::Collide(Collidable* _Collidable)
{
	// If we hit a snake body
	if (dynamic_cast<SnakeBody*>(_Collidable) != nullptr)
	{
		mGame->QueueMessage("GameEvents", std::make_unique<Message>(MESSAGE_TYPE_DOUBLE, (double)COLLISION_WITH_SELF));
	}
	// If we hit a snake bit
	if (dynamic_cast<SnakeBitObject*>(_Collidable) != nullptr)
	{
		mGame->QueueMessage("GameEvents", std::make_unique<Message>(MESSAGE_TYPE_DOUBLE, (double)COLLISION_WITH_BIT));
		AddBody();
	}
}

void SnakeHead::CollisionBroadcast()
{
	mGame->QueueMessage("Collision", std::make_unique<Message>(Message(MESSAGE_TYPE_COLLISION_PTR, dynamic_cast<GameObject*>(this))));
}