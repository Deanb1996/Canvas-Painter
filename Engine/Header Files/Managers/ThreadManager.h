#pragma once
#include "Thread.h"
#include <vector>
#include <memory>

class ThreadManager
{
private:
	std::vector<Thread*> mThreads;
	std::queue<Task*> mTasks;

	//Private constructor for singleton pattern
	ThreadManager();

public:
	~ThreadManager();

	//Singleton pattern
	//Deleted copy constructor and assignment operator so no copies of the singleton instance can be made
	ThreadManager(const ThreadManager& ThreadManager) = delete;
	ThreadManager& operator=(ThreadManager const&) = delete;

	const Task* const AddTask(void(*pFunction)(), const int pThreadAffinity);
	void ProcessTasks();

	static std::shared_ptr< ThreadManager > Instance();
};