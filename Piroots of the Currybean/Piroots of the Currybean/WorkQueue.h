#pragma once
#include <queue>
#include <mutex>

template<typename T>

class CWorkQueue {
public:
	CWorkQueue() {};

	//Pushes a task to the back of the queue
	void push(const T& item) {
		//Lock the queue so that it is atomic so that we can push safely
		std::lock_guard<std::mutex> _lock(QueueMutex);
		WorkQueue.push(item);
	}

	//Attempt to get a workitem from the queue
	bool pop(T& _workItem) {
		//Lock the queue so that operations are atomic and we can pop safely
		std::lock_guard<std::mutex> _lock(QueueMutex);

		//If the work queue is empty, return false
		if (WorkQueue.empty()) return false;
		_workItem = WorkQueue.front();
		WorkQueue.pop();
		return true;
	}

	//Checking if the queue is empty or not
	bool empty() const {
		//locking the queue so that we can perform operations on it safely
		std::lock_guard<std::mutex> _lock(QueueMutex);
		return WorkQueue.empty();
	}
private:
	std::queue<T> WorkQueue;
	mutable std::mutex QueueMutex;
};
