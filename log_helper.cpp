#include "log_helper.h"

#include <iostream>
#include <mutex>

static std::mutex log_mtx;

void thread_safe_log(std::string && s)
{
	std::lock_guard<std::mutex> lock(log_mtx);

	std::cerr << s;
}
