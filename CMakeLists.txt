cmake_minimum_required (VERSION 3.8)
project ("Задача 1")

set(CMAKE_CXX_STANDARD 11)

set(
	READERS_AND_WRITERS_HEADER_FILES
		winapi_typedefs.h
		thread.h
		mutex.h
		lock_guard.h
		utils.h
		settings.h
)


set(
	READERS_AND_WRITERS_SOURCE_FILES
		main.cpp
		mutex.cpp
)

add_executable (
	ReadersAndWriters 
		${READERS_AND_WRITERS_HEADER_FILES}
		${READERS_AND_WRITERS_SOURCE_FILES}
)

