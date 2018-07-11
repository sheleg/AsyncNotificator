#pragma once
#include <iostream>
#include <queue>
#include <thread>
#include <chrono> 
#include <mutex>  


template<typename Message, typename Callback = void(*)(Message const&)>
class base_notificator
{
protected:
	Callback callback_function_;
	std::queue<Message> message_queue_;
	bool is_thread_stopped_ = false;
	std::condition_variable cv_;
	std::mutex mtx_;

private:
	std::thread thread_;

	void listener_function()
	{
		while (true) {
			std::queue<Message> temp_message_queue;

			{
				std::unique_lock<std::mutex> u_lock(mtx_);
				cv_.wait(u_lock, [this]() { return (!message_queue_.empty()) || is_thread_stopped_; });
				std::swap(temp_message_queue, message_queue_);
			}

			while (!temp_message_queue.empty())
			{

				callback_function_(temp_message_queue.front());
				temp_message_queue.pop();
			}

			if (is_thread_stopped_)
			{
				break;
			}
		}
	}

public:
	base_notificator(Callback function)
		: callback_function_(function)
		, thread_(&base_notificator::listener_function, this) {}

	~base_notificator()
	{
		{
			std::lock_guard<std::mutex> l_guard(mtx_);
			is_thread_stopped_ = true;
			cv_.notify_all();
		}

		thread_.join();
	}

	virtual void notify(const Message& msg) = 0;
};