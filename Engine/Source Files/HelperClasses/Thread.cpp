#include "Thread.h"

/// <summary>
/// The main function for the thread
/// Calls the run function of the thread class
/// </summary>
auto threadMain = [](Thread* pThread)
{
	pThread->Run();
};

/// <summary>
/// Constructor for the thread class
/// Calls the start method upon construction, creating a new thread
/// </summary>
Thread::Thread()
	:mTask(nullptr), mNeedsTask(true)
{
	Start();
}

/// <summary>
/// Default destructor
/// </summary>
Thread::~Thread()
{
}

/// <summary>
/// Creates a C++ std:thread that runs the threadMain function and passes this thread class as a parameter so it can call the functions of the thread class
/// </summary>
void Thread::Start()
{
	mThread = std::thread(threadMain, this);
}

/// <summary>
/// Sets the task of the thread to a given task
/// </summary>
/// <param name="pTask">The given task to assign to the thread</param>
void Thread::SetTask(Task* pTask)
{
	mTask = pTask;
	mNeedsTask = false;
}

/// <summary>
/// The run method of the thread that contains all the logic executed on the thread
/// </summary>
void Thread::Run()
{
	while (true)
	{
		if (mTask)
		{
			//Sets thread affinity to the affinity specified in the task, runs the task and then sets thread to requiring new task
			SetThreadAffinity(mTask->ThreadAffinity());
			mTask->Run();
			mTask = nullptr;
			mNeedsTask = true;
		}

		//Sleep to prevent thread taking 100% cpu with a busy while
		Sleep(1);
	}
}

/// <summary>
/// Get method for the Threads need task status
/// </summary>
/// <returns>A bool determining whether the thread needs a task or not</returns>
bool Thread::NeedsTask()
{
	return mNeedsTask;;
}

/// <summary>
/// Sets the thread affinity to the given core
/// </summary>
/// <param name="pCore">Core to set the threads affinity to</param>
void Thread::SetThreadAffinity(const std::vector<int>& pCores)
{
	if (pCores.size() != 0)
	{
		const auto handle = mThread.native_handle();
		DWORD_PTR mask = 0;

		for (const auto& core : pCores)
		{
			if (!(core < 0))
			{
				mask += (1i64 << core);
			}
		}
		SetThreadAffinityMask(handle, mask);
	}
}
