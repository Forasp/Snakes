#include "Listener.h"
#include "Messenger.h"

Listener::~Listener()
{
	if (mAttachedMessengers.size() == 0u)
	{
		return;
	}

	for (int i = mAttachedMessengers.size() - 1; i >= 0; i--)
	{
		RemoveFromMessenger(mAttachedMessengers[i]);
	}

	mAttachedMessengers.clear();
}

void Listener::AttachToMessenger(Messenger* _Messenger)
{
	for (Messenger* i : mAttachedMessengers)
	{
		if (i == _Messenger)
		{
			return;
		}
	}

	mAttachedMessengers.push_back(_Messenger);
	_Messenger->AddListener(this);
}

// Removes the local pointer to a messenger and the messenger's pointer to this
void Listener::RemoveFromMessenger(Messenger* _Messenger)
{
	if (mAttachedMessengers.size() == 0u)
	{
		return;
	}

	for (std::vector<Messenger*>::iterator it = mAttachedMessengers.begin(); it != mAttachedMessengers.end(); ++it)
	{
		if (*it == _Messenger)
		{
			mAttachedMessengers.erase(it);
			if (_Messenger != nullptr)
			{
				_Messenger->RemoveListener(this);
			}
			return;
		}
	}
}