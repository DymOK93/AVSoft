#pragma once
#include "winapi_typedefs.h"
#include <utility>
#include <stdexcept>

namespace winapi_wrappers {
	class mutex {
	public:
		mutex();
		mutex(const mutex&) = delete;
		mutex& operator=(const mutex&) = delete;
		~mutex();

		void lock();
		void unlock();
	private:
		static void throw_mutex_create_error();
	private:
		handle_t m_mutex;
	};

	class shared_mutex : public mutex {
	public:
		shared_mutex();
		shared_mutex(const shared_mutex&) = delete;
		shared_mutex& operator=(const shared_mutex&) = delete;
		~shared_mutex();

		void lock();
		void unlock();
		void lock_shared();
		void unlock_shared();
	private:
		handle_t read, owned;
		atomic32_t counter{ 0 };
	};

}
