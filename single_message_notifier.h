#pragma once
#include "notificator.h"

template<typename Message, typename Callback = void(*)(Message)>
class  single_message_notifier : public base_notificator<Message, Callback>
{
	using base_notificator<Message, Callback>::mtx_;
	using base_notificator<Message, Callback>::message_queue_;
	using base_notificator<Message, Callback>::cv_;

public:
	single_message_notifier(Callback function)
		:base_notificator<Message, Callback>(function) {}

	void notify(const Message& msg)
	{
		std::lock_guard<std::mutex> l_guard(mtx_);
		
		if (!message_queue_.empty())
		{
			message_queue_.pop();
		}

		message_queue_.push(msg);
		cv_.notify_all();
	}
};