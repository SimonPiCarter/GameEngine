#pragma once

#include <list>
#include <mutex>

///
/// @brief This class is the base class used
/// to handle thread safe messages
/// Use two list of Message
/// - One is static and only used to pop messages and handle them (_handleMsg)
/// - One is dynamic and only used to add new messages (_inputMsg)
///
template<typename Message>
class MessageHandler
{
public:
	MessageHandler() {}
	virtual ~MessageHandler()
	{
		// clean up memory
		for(Message *msg_l : _inputMsg)
		{
			delete msg_l;
		}
		for(Message *msg_l : _handleMsg)
		{
			delete msg_l;
		}
	}

	/// @brief register new message in _inputMsg
	void registerMessage(Message * msg_p)
	{
		std::lock_guard<std::mutex> lock_l(_inputMsgMutex);
		_inputMsg.push_back(msg_p);
	}

	/// @brief swap _inputMsg and _handleMsg
	/// @remark should only be called when all messages have been poped
	void swapMessages()
	{
		std::lock_guard<std::mutex> lockHandle_l(_handleMsgMutex);
		if(_handleMsg.empty())
		{
			std::lock_guard<std::mutex> lockInput_l(_inputMsgMutex);
			std::swap(_handleMsg, _inputMsg);
		}
		else
		{
			throw std::logic_error("Cannot call swapMessages on non empty _handleMsg");
		}
	}

	/// @brief pop first message of _handleMsg
	/// @return true iif _handleMsg is not empty after pop
	bool popMessage()
	{
		std::lock_guard<std::mutex> lock_l(_handleMsgMutex);
		if(!_handleMsg.empty())
		{
			Message * msg_l = _handleMsg.front();
			_handleMsg.pop_front();
			handleMessage(msg_l);
			delete msg_l;
		}
		// true iif not empty
		return !_handleMsg.empty();
	}

	/// @brief will pop all message and swap list of messages
	void handleAllMessages()
	{
		// handle all messages
		while(popMessage())
		{
			// NA (handle of message is done in popMessage)
		}
		swapMessages();
	}

	/// @brief check if message handler has messahe to handle
	bool hasMessage()
	{
		return !_handleMsg.empty() || !_inputMsg.empty();
	}

private:
	/// @brief list of Message to be handled
	std::list<Message *> _handleMsg;
	/// @brief list of Message where new Message are added
	std::list<Message *> _inputMsg;

	/// @brief thread safety
	std::mutex _handleMsgMutex;
	/// @brief thread safety
	std::mutex _inputMsgMutex;

protected:
	/// @brief method to be extended to handle message
	virtual void handleMessage(Message *msg_p) = 0;
};
