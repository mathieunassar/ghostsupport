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

 /// returns a reference to the next element of the queue, but does not remove it. If the queue is empty, this call is blocking.
template <typename T>
const T& BlockingQueue<T>::get() const
{
	std::unique_lock<std::mutex> lock(_mutex);
	_condition.wait(lock, [=] { return !_queue.empty(); });

	return _queue.back();
}

template <typename T>
template<class Rep, class Period>
bool BlockingQueue<T>::tryGet(std::chrono::duration<Rep, Period> timeout, T& result)
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (_queue.size() == 0)
		_condition.wait_for(lock, timeout, [=] { return !_queue.empty(); });

	if (_queue.size() == 0)
		return false;

	result = _queue.back();
	return true;
}

template <typename T>
T BlockingQueue<T>::getAndPop()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_condition.wait(lock, [=] { return !_queue.empty(); });

	T res = _queue.back();
	_queue.pop_back();
	return std::move(res);
}

template <typename T>
template<class Rep, class Period>
bool BlockingQueue<T>::tryGetAndPop(std::chrono::duration<Rep, Period> timeout, T& result)
{
	return tryGetAndPop(std::chrono::duration_cast<std::chrono::nanoseconds>(timeout), result);
}

template <typename T>
bool BlockingQueue<T>::tryGetAndPop(std::chrono::nanoseconds timeout, T& result)
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (_queue.size() == 0)
		_condition.wait_for(lock, timeout, [=] { return !_queue.empty(); });

	if (_queue.size() == 0)
		return false;

	result = std::move(_queue.back());
	_queue.pop_back();
	return true;
}


/// adds a value to the queue.
template <typename T>
void BlockingQueue<T>::push(const T& value)
{
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_queue.push_front(value);
	}
	_condition.notify_one();
}

/// removes the next element of the queue. If the queue is empty, does not do anything.
template <typename T>
void BlockingQueue<T>::pop()
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (!_queue.empty()) // calling pop_back on an empty queue is undefined
		_queue.pop_back();
}

/// returns the size of the queue.
template <typename T>
size_t BlockingQueue<T>::size() const
{
	std::unique_lock<std::mutex> lock(_mutex);
	return _queue.size();
}
