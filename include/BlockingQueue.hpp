#ifndef GHOST_INTERNAL_BLOCKINGQUEUE_HPP
#define GHOST_INTERNAL_BLOCKINGQUEUE_HPP

#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <future>
#include <memory>

namespace ghost
{
	namespace internal
	{
		template<typename T>
		class QueueElement
		{
		public:
			T element;
			std::shared_ptr<std::promise<bool>> result;
		};

		// inspired from https://stackoverflow.com/questions/12805041/c-equivalent-to-javas-blockingqueue
		template <typename T>
		class BlockingQueue
		{
		public:
			/// returns a reference to the next element of the queue, but does not remove it. If the queue is empty, this call is blocking.
			const T& get() const;
			/// fetches the next element of the queue, if one is available within the given time. Returns true if an element was available.
			bool tryGet(std::chrono::milliseconds timeout, T& result);
			/// adds a value to the queue.
			void push(const T& value);
			/// removes the next element of the queue. If the queue is empty, does not do anything.
			void pop();
			/// returns the size of the queue.
			size_t size() const;

		private:
			mutable std::mutex _mutex;
			mutable std::condition_variable _condition;
			std::deque<T> _queue;
		};

		#include "BlockingQueue.impl.hpp"
	}
}

#endif //GHOST_INTERNAL_BLOCKINGQUEUE_HPP
