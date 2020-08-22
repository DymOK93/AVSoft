#include "mutex.h"
using namespace std;

namespace winapi_wrappers {
	mutex::mutex()
		: m_mutex{
		CreateMutex(
			/*lpMutexAttributes*/	NULL,
			/*bInitialOwner*/		false,
			/*lpName*/				NULL
		)} {
		if (!m_mutex) {
			throw_mutex_create_error();
		}
	}

	mutex::~mutex() {
		CloseHandle(m_mutex);
	}

	void mutex::lock() {
		WaitForSingleObject(
			m_mutex, 
			INFINITE
		);
	}

	void mutex::unlock() {
		ReleaseMutex(m_mutex);
	}



	void mutex::throw_mutex_create_error() {
		throw runtime_error("Can't create mutex");
	}

	shared_mutex::shared_mutex()
		: read{
		CreateEvent(
			/*lpEventAttributes*/	NULL,
			/*lpEventAttributes*/	true,
			/*bInitialState */		true,
			/*lpName*/				NULL
		) },
		owned{
		CreateEvent(
			/*lpEventAttributes*/	NULL,
			/*lpEventAttributes*/	true,
			/*bInitialState */		true,
			/*lpName*/				NULL
		) }
	{
	}

	shared_mutex::~shared_mutex() {
		CloseHandle(read);
		CloseHandle(owned);
	}

	void shared_mutex::lock() {
		mutex::lock();											//«акрытие доступа дл€ других писателей
		ResetEvent(owned);
		WaitForSingleObject(									//ќжидание завершени€ работы читателей, уже начавших операцию
			read, 
			INFINITE
		);
	}

	void shared_mutex::unlock() {
		SetEvent(owned);										
		mutex::unlock();
	}

	void shared_mutex::lock_shared() {
		WaitForSingleObject(
			owned,
			INFINITE
		);	
		ResetEvent(read);										//«акрытие доступа писател€м
		InterlockedIncrement(addressof(counter));
	}

	void shared_mutex::unlock_shared() {
		if (InterlockedDecrement(addressof(counter)) == 0) {
			SetEvent(read);										//ќткрытие доступа писател€м
		}
	}
}