#pragma once

namespace winapi_wrappers {
	template <class Mutex>
	class lock_guard {
	public:
		explicit lock_guard(Mutex& mutex)
			: m_mutex(mutex) {
			m_mutex.lock();
		}

		lock_guard(const lock_guard&) = delete;
		lock_guard& operator=(const lock_guard&) = delete;

		~lock_guard() {
			m_mutex.unlock();
		}
	private:
		Mutex& m_mutex;
	};

	template <class Mutex>
	class shared_lock_guard {
	public:
		explicit shared_lock_guard(Mutex& mutex)
			: m_mutex(mutex) {
			m_mutex.lock_shared();
		}

		shared_lock_guard(const shared_lock_guard&) = delete;
		shared_lock_guard& operator=(const shared_lock_guard&) = delete;

		~shared_lock_guard() {
			m_mutex.unlock_shared();
		}
	private:
		Mutex& m_mutex;
	};
}