#pragma once
#include "notificator.h"

template<typename Message, typename Callback = void(*)(Message)>
class buffered_messages_notifier : public base_notificator<Message, Callback>
{
	using base_notificator<Message, Callback>::mtx_;
	using base_notificator<Message, Callback>::message_queue_;
	using base_notificator<Message, Callback>::cv_;

public:
	buffered_messages_notifier(Callback function)
		:base_notificator<Message, Callback>(function) {}

	void notify(const Message& msg)
	{
		std::lock_guard<std::mutex> l_guard(mtx_);
		message_queue_.push(msg);
		cv_.notify_all();
	}
};