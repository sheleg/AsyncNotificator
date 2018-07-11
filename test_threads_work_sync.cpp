#include "buffered_messages_notifier.h"
#include "log_helper.h"
#include <set>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <sstream>

constexpr int worker_call_count = 5;
constexpr int worker_sleep_in_milliseconds = 5;
constexpr int main_wait_in_milliseconds = 5000;

using notificator_type = buffered_messages_notifier<int, void(*)(int)>;
static std::set<int> data_to_check;

static void callback_func(int n)
{
	data_to_check.insert(n);
}

static void worker(std::shared_ptr<notificator_type> notificator_ptr, int n)
{
	for (int i = 0, k = n; i != worker_call_count; ++i, k += 2)
	{
		(*notificator_ptr).notify(k);
		std::this_thread::sleep_for(std::chrono::milliseconds(worker_sleep_in_milliseconds));
	}
}

class test_threads_work_sync
{
public:
	test_threads_work_sync()
	{
		auto ptr = std::make_shared<notificator_type>(callback_func);
		std::set<int> true_data;
		int n = 1;
		std::generate_n(std::inserter(true_data, true_data.begin()), 10, [&n]() { return n++; });
		std::thread t1(worker, ptr, 1);
		std::thread t2(worker, ptr, 2);
		t1.join();
		t2.join();
		std::this_thread::sleep_for(std::chrono::milliseconds(main_wait_in_milliseconds));
		assert(true_data == data_to_check);
	}
};

static test_threads_work_sync the_test;

