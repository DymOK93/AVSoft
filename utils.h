#pragma once
#include "mutex.h"
#include "thread.h"
#include "lock_guard.h"
#include "settings.h"

#include <iostream>
#include <vector>


void eternal_reader(size_t iterations_count, size_t sleep_time);
void eternal_writer(size_t iterations_count, size_t sleep_time);

struct emulation_settings {
	const size_t
		readers_count,
		writers_count,
		reader_delay,
		writer_delay,
		iterations_count;
};



void start_emulation(const emulation_settings& settings);
void wait_for_all_workers(std::vector<winapi_wrappers::thread>& workers);

template <class Func, class... Types>
std::vector<winapi_wrappers::thread> make_workers_vector(
	size_t count,
	const Func& func,
	const Types&... args
) {
	std::vector<winapi_wrappers::thread> workers;
	workers.reserve(count);
	for (size_t idx = 0; idx < count; ++idx) {
		workers.emplace_back(
			func,									//¬ рамках конкретной задачи допустимо прин€ть const Ty& и просто копировать аргументы, без perfect forwarding
			args...									
		);
	}
	return workers;
}

