#include "ThreadManager.h"

/// <summary>
/// Constructor
/// Creates number of threads equal to double the available hardware cores
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
/// Destructor
/// Cleans up all the threads from memory
/// </summary>
ThreadManager::~ThreadManager()
{
	for (auto& thread : mThreads)
	{
		delete thread;
	}
}

/// <summary>
/// Adds a new task to the task queue
/// </summary>
/// <param name="pFunction">std::function containing function pointer to a function that holds the task</param>
/// <param name="pParam1">First parameter of the function</param>
/// <param name="pParam2">Second parameter of the function</param>
/// <param name="pThreadAffinity">Thread affinity to set the task to</param>
/// <returns>A handle to the created task so tha the tasks completion and be monitored and the memory can be cleaned up upon completion</returns>
Task* const  ThreadManager::AddTask(std::function<void(void* param1, void* param2)> pFunction, void* pParam1, void* pParam2, const std::vector<int>& pThreadAffinity)
{
	Task* task = new Task(pFunction, pParam1, pParam2, pThreadAffinity);

	mTasks.push(task);

	return task;
}

/// <summary>
/// Assigns tasks from the queue to any threads that need tasks if there any tasks to be done
/// </summary>
void ThreadManager::ProcessTasks()
{
	for (auto thread : mThreads)
	{
		if (mTasks.size() > 0)
		{
			//If thread needs tasks, set threads task to front of task queue, then pop task off queue
			if (thread->NeedsTask())
			{
				thread->SetTask(mTasks.front());
				mTasks.pop();
			}
		}
	}
}

/// <summary>
/// If an instance of the thread manager does not already exists, creates one and then provides a pointer to it
/// </summary>
/// <returns>Pointer to the thread manager instance</returns>
std::shared_ptr<ThreadManager> ThreadManager::Instance()
{
	static std::shared_ptr<ThreadManager> instance{ new ThreadManager() };
	return instance;
}
