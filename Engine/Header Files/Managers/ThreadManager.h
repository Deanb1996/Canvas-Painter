#pragma once
#include "Thread.h"
#include <vector>
#include <memory>
#include <queue>
#include <mutex>

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

	Task* const AddTask(std::function<void(void* param1, void* param2)> pFunction, void* pParam1, void* pParam2, const std::vector<int>& pThreadAffinity);
	void ProcessTasks();

	static std::shared_ptr< ThreadManager > Instance();
};