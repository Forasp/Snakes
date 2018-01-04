#include "Game.h"
#include "Messenger.h"
#include "Message.h"
#include "SnakeBitObject.h"
#include "SnakeHead.h"
#include "GlobalDefines.h"
#include "GameThreadUnsafeScope.h"

void SnakeBitObject::InitializeGameObject()
{
	mBitRadius = 20;
	mRotation = 45;
	mLayer = LAYER_GAME_BACKGROUND;
	Listener::AttachToMessenger(mGame->GetMessenger("KeyEvents").get());
	mRenderShape = sf::CircleShape(mBitRadius, 4);
	mSize = std::make_pair(mBitRadius * 2, mBitRadius * 2);
	CreateCollider(&mPosition, &mSize, &mRotation);
	//mSnakeHeadRect.setRotation(45);
	mPosition.first = 100;
	mPosition.second = 100;
	mRenderShape.setPosition(mPosition.first, mPosition.second);
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

SnakeBitObject::~SnakeBitObject()
{
	if (mGame != nullptr && mAddedToRenderer)
	{
		GameThreadUnsafeScope ScopeLock(mGame);
		mGame->RemoveObjectFromRenderer(this, mLayer);
		mAddedToRenderer = false;
	}
}

void SnakeBitObject::RenderTick(sf::RenderWindow* _RenderWindow)
{
	mRenderShape.setPosition(mPosition.first, mPosition.second);
	_RenderWindow->draw(mRenderShape);

	//if (mSnakeBody != nullptr)
	//{
	//	mSnakeBody->RenderTick(_RenderWindow);
	//}
}

void SnakeBitObject::HandleMessage(Message* _Message)
{

}

void SnakeBitObject::Tick(sf::Time _DeltaTime)
{
	GameObject::Tick(_DeltaTime);
}

void SnakeBitObject::Collide(Collidable* _Collidable)
{
	// If we hit a snake body
	if (dynamic_cast<SnakeHead*>(_Collidable) != nullptr)
	{
		// Move to random location
		mPosition.first = rand() % 900 + 62;
		mPosition.second = rand() % 644 + 62;
	}
}