#pragma once
#include <vector>
#include <memory>

class Message;
class Messenger;

class Listener
{
public:
	void AttachToMessenger(Messenger* _Messenger);
	virtual void ReadMessage(Message* _Message) = 0;
	void RemoveFromMessenger(Messenger* _Messenger);
protected:
	~Listener();
	std::vector<Messenger*> mAttachedMessengers;
};