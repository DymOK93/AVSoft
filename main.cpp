#include "utils.h"
using winapi_wrappers::thread;
using winapi_wrappers::shared_mutex;
using winapi_wrappers::lock_guard;
using winapi_wrappers::shared_lock_guard;

shared_mutex mtx;

int main()
{
	start_emulation(
		emulation_settings{
			READERS_COUNT,
			WRITERS_COUNT,
			READER_DELAY,
			WRITER_DELAY,
			ITERATIONS_COUNT
		}
	);
	return 0;
}

void eternal_reader(size_t iterations_count, size_t sleep_time) {
	const winapi_wrappers::id_t thread_id { GetCurrentThreadId() };
	while (iterations_count--) {
		std::cout << "I'm reader from thread " << thread_id << " and I'm checking the lock\n";
		shared_lock_guard<shared_mutex> slg(mtx);

		std::cout << "I'm reader from thread " << thread_id << " and I'm reading data\n";
		Sleep(sleep_time);
		std::cout << "I'm reader from thread " << thread_id << " and I'm finished my work\n";
	}	
}

void eternal_writer(size_t iterations_count, size_t sleep_time) {
	const winapi_wrappers::id_t thread_id{ GetCurrentThreadId() };

	while (iterations_count--) {
		std::cout << "I'm writer from thread " << thread_id << " and I'm checking the lock\n";
		lock_guard<shared_mutex> lg(mtx);

		std::cout << "I'm writer from thread " << thread_id << " and I'm writing data\n";
		Sleep(sleep_time);
		std::cout << "I'm writer from thread " << thread_id << " and I'm finished my work\n";
	}
}

void start_emulation(const emulation_settings& settings) {
	auto readers{
		make_workers_vector(
			settings.readers_count,
			eternal_reader,
			settings.iterations_count,
			settings.reader_delay
		)
	},
		writers{
		make_workers_vector(
			settings.writers_count,
			eternal_writer,
			settings.iterations_count,
			settings.writer_delay
		)
	};

	wait_for_all_workers(readers);									
	wait_for_all_workers(writers);									
} 

void wait_for_all_workers(std::vector<winapi_wrappers::thread>& workers) {
	for (auto& worker : workers) {									//Одно из немногих мест, где с использованием чистого WinAPI
		worker.join();												//можно сделать проще и лучше: WaitForMultipleObjects()
	}
}