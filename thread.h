#pragma once
#include "winapi_typedefs.h"

#include <tuple>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <functional>

namespace winapi_wrappers {
	struct thread_info {
		handle_t handle;
		id_t id;
	};

	class thread {
	public:
		thread() noexcept = default;														//����������� �� ���������

		template <class Func, class... Types>
		thread(Func&& func, Types&&... args) {
			using Tuple = std::tuple<std::decay_t<Func>, std::decay_t<Types>...>;			
			m_thread = create_thread<Tuple, sizeof...(args) + 1>(							//sizeof...(args) - ����� ���������� � ������ ����������
				std::make_unique<Tuple>(
					std::forward<Func>(func),
					std::forward<Types>(args)...
				)
			);
			if (!joinable()) {																
				throw_thread_create_error();
			}
		}

		thread(const thread&) = delete;														//���������� ������� ������� ���������
		thread(thread&& other) noexcept
			: m_thread{ std::exchange(other.m_thread, {}) }
		{
		}

		thread& operator=(const thread&) = delete;
		thread& operator=(thread&& other) {
			terminate_if_joinable();
			m_thread = std::exchange(other.m_thread, {});
			return *this;
		}

		~thread() {
			terminate_if_joinable();
		}

		void join() {
			if (!joinable()) {
				throw_empty_thread();
			}
			if (m_thread.id == GetCurrentThreadId()) {
				throw_bad_join_to_myself();
			}
			wait_thread(m_thread.handle);
			close_thread(m_thread.handle);														//�� �������� ������� ���������� ������
			m_thread = {};
		}

		bool joinable() const noexcept {
			return m_thread.handle && m_thread.id;
		}

		id_t get_id() const noexcept {
			return m_thread.id;
		}
	private:
		/************************************************************************
		��������� �� ����� ������ � ��������� ���������� �����������
		�������� ����� ��������������� ������� index_unpack_helper.
		����� �������� release() � unique_ptr ����������� - ������� ������� ������
		��� ��������� �������� ������.
		���� �������� �������, �������� �������� �� ��������� �������� ��������.
		**************************************************************************/
		template<class Tuple, size_t tuple_elements_count>
		static thread_info create_thread(std::unique_ptr<Tuple> func_args) {
			thread_info new_thread{};
			new_thread.handle = CreateThread(
				/*lpThreadAttributes*/	NULL,
				/*dwStackSize*/			0,		
				/*lpStartAddress*/		index_unpack_helper<Tuple>(std::make_index_sequence<tuple_elements_count>{}),   						
				/*lpParameter*/			func_args.get(),		
				/*dwCreationFlags*/		0,
				/*lpThreadId*/			std::addressof(new_thread.id)
			);
			if (new_thread.handle) {
				func_args.release();															
			}
			else {
				throw_thread_create_error();
			}
			return new_thread;
		}

		template <class Tuple, size_t... Indices>
		static constexpr auto index_unpack_helper(std::index_sequence<Indices...> idcs) {
			return &invoke_helper<Tuple, Indices...>;											//index_sequence �������� ����� �� 0 �� tuple_elements_count-1	
		}

		template <class Tuple, size_t... Indices>
		static ret_t invoke_helper(arg_t arg) {
			const std::unique_ptr<Tuple> raw_args(reinterpret_cast<Tuple*>(arg));				//�������� ������ �� ��������
			auto& arg_tuple{ *raw_args };
			std::invoke(std::move(std::get<Indices>(arg_tuple))...);							//����� ���������������� ������� � ���������� �����������
			return 0;
		}

		static ret_t wait_thread(handle_t handle) {
			return WaitForSingleObject(
				handle, 
				INFINITE
			);
		}

		static bool_t close_thread(handle_t handle) {
			return CloseHandle(handle);
		}
	private:
		/******************************
		��������� ���������� ��������
		******************************/
		void terminate_if_joinable() const {
			if (joinable()) {
				std::terminate();
			}
		}

		static void throw_thread_create_error() {
			throw std::runtime_error("Can't create a thread");
		}

		static void throw_empty_thread() {
			throw std::runtime_error("Empty thread");
		}

		static void throw_bad_join_to_myself() {
			throw std::runtime_error("Can't join to myself");
		}
	private:
		thread_info m_thread;
	};
}