#include "ThreadManager.h"

/// <summary>
/// 
/// </summary>
ThreadManager::ThreadManager()
{
	int maxThreads = static_cast<int>(std::thread::hardware_concurrency() * 2);

	for (int i = 0; i < maxThreads; i++)
	{
		mThreads.push_back(new Thread());
	}
}

/// <summary>
/// 
/// </summary>
ThreadManager::~ThreadManager()
{
	for (auto& thread : mThreads)
	{
		delete thread;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="pFunction"></param>
/// <param name="pThreadAffinity"></param>
const Task* const  ThreadManager::AddTask(void(*pFunction)(), const int pThreadAffinity)
{
	Task* task = new Task(pFunction, pThreadAffinity);

	mTasks.push(task);

	return task;
}

/// <summary>
/// 
/// </summary>
void ThreadManager::ProcessTasks()
{
	for (auto thread : mThreads)
	{
		if (mTasks.size() > 0)
		{
			if (thread->NeedsTask())
			{
				thread->SetTask(mTasks.front());
				mTasks.pop();
			}
		}
	}
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::shared_ptr<ThreadManager> ThreadManager::Instance()
{
	static std::shared_ptr<ThreadManager> instance{ new ThreadManager() };
	return instance;
}
