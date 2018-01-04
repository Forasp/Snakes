#include "Messenger.h"
#include "Message.h"

/// <summary> 
/// Adds a listener to the Messenger. 
/// </summary>
void Messenger::AddListener(Listener* _Listener)
{
	for (Listener* i : mListeners)
	{
		if (i == _Listener)
		{
			return;
		}
	}

	mListenersMutex.lock();
	mListeners.push_back(_Listener);
	mListenersMutex.unlock();
	_Listener->AttachToMessenger(this);
}

/// <summary> 
/// Remove a listener from the Messenger 
/// </summary>
void Messenger::RemoveListener(Listener* _Listener)
{
	if (mListeners.size() == 0u)
	{
		return;
	}

	for (std::vector<Listener*>::iterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		if (*it == _Listener)
		{
			mListenersMutex.lock();
			mListeners.erase(it);
			mListenersMutex.unlock();
			_Listener->RemoveFromMessenger(this);
			return;
		}
	}
}

/// <summary> 
/// Receive a message and distribute to all listeners.
/// </summary>
void Messenger::QueueMessage(std::shared_ptr<Message> _Message)
{
	// lock the queue when pushing or popping.
	mWritingMessageLock.lock();
	mIncomingMessageQueue.push(std::move(_Message));
	mWritingMessageLock.unlock();
}

void Messenger::TickMessenger()
{
	while (mMessageQueue.size() > 0)
	{
		Message* MessageFront = mMessageQueue.front().get();
		for (int i = 0; i < mListeners.size(); i++)
		{
			if (mListeners[i] != nullptr)
			{
				mListeners[i]->ReadMessage(MessageFront);
			}
		}
		
		mMessageQueue.pop();
	}
	while (mIncomingMessageQueue.size() > 0)
	{
		// lock the queue when pushing or popping.
		mWritingMessageLock.lock();
		mMessageQueue.push(mIncomingMessageQueue.front());
		mIncomingMessageQueue.pop();
		mWritingMessageLock.unlock();
	}
}