cmake_minimum_required (VERSION 3.8)
project ("Задача 1")

set(CMAKE_CXX_STANDARD 11)

add_executable (
	ReadersAndWriters 
		main.cpp
		mutex.cpp
)

