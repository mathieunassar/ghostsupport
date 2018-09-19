/// returns a reference to the next element of the queue, but does not remove it. If the queue is empty, this call is blocking.
template <typename T>
const T& BlockingQueue<T>::get() const
{
	std::unique_lock<std::mutex> lock(_mutex);
	_condition.wait(lock, [=] { return !_queue.empty(); });
	
	return _queue.back();
}

/// fetches the next element of the queue, if one is available within the given time. Returns true if an element was available.
template <typename T>
bool BlockingQueue<T>::tryGet(std::chrono::milliseconds timeout, T& result)
{
	std::unique_lock<std::mutex> lock(_mutex);
	if (_queue.size() == 0)
		_condition.wait_for(lock, timeout, [=] { return !_queue.empty(); });

	if (_queue.size() == 0)
		return false;

	result = _queue.back();
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
