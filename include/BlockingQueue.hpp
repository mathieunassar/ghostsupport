/*
 * Copyright 2019 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
			/// Performs a get, and a pop.
			T getAndPop() const;
			/// Performs a tryGet, and a pop.
			bool tryGetAndPop(std::chrono::milliseconds timeout, T& result) const
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
