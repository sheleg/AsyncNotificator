#include"single_message_notifier.h"
#include "log_helper.h"
#include <set>
#include <iostream>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <sstream>

constexpr int worker_call_count = 50;
constexpr int worker_sleep_in_milliseconds = 5;
constexpr int main_wait_in_milliseconds = 5000;

using notificator_type = single_message_notifier<int, void(*)(int)>;
static std::set<int> data_to_check_1;

static void thread_callback_func(int n)
{
	data_to_check_1.insert(n);
}

static void thread_worker(std::shared_ptr<notificator_type> notificator_ptr, int n)
{
	for (int i = 0, k = n; i != worker_call_count; ++i, k += 2)
	{
		(*notificator_ptr).notify(k);
	}
}

class test_single_message_thread_sync
{
public:
	test_single_message_thread_sync()
	{
		auto ptr = std::make_shared<notificator_type>(thread_callback_func);
		std::set<int> true_data;
		int n = 1;
		std::generate_n(std::inserter(true_data, true_data.begin()), 100, [&n]() { return n++; });
		std::thread t1(thread_worker, ptr, 1);
		std::thread t2(thread_worker, ptr, 2);
		t1.join();
		t2.join();
		std::this_thread::sleep_for(std::chrono::milliseconds(main_wait_in_milliseconds));
		assert(true_data != data_to_check_1);
	}
};

static test_single_message_thread_sync the_test;

