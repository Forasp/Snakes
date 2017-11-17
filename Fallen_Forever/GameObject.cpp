#include "GameObject.h"
#include "Game.h"
#include "Message.h"
#include "GameThreadUnsafeScope.h"

GameObject::~GameObject()
{
	if (mGame != nullptr && mAddedToRenderer)
	{
		GameThreadUnsafeScope ScopeLock(mGame);
		mGame->RemoveObjectFromRenderer(this, mLayer);
		mAddedToRenderer = false;
	}
}

void GameObject::RenderTick(sf::RenderWindow* _RenderWindow)
{
	if (!mAddedToRenderer)
	{
		mGame->AddObjectToRenderer(this, mLayer);
		mAddedToRenderer = true;
	}

	mRenderStates.transform = getTransform();
	_RenderWindow->draw(mSprite, mRenderStates);
}

void GameObject::SetTexResources(TextureResources* _ResourceHolder)
{
	mResourceHolder = _ResourceHolder;
}

void GameObject::Tick(sf::Time _DeltaTime)
{
	float TimeMult = _DeltaTime.asSeconds();
	mPosition.first += mVelocity.first * TimeMult;
	mPosition.second += mVelocity.second * TimeMult;

	for (std::shared_ptr<GameObject> it : mChildren)
	{
		it->Tick(_DeltaTime);
	}
}