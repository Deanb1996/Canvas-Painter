#include "Thread.h"

auto threadMain = [](Thread* pThread)
{
	pThread->Run();
	return 0;
};

/// <summary>
/// 
/// </summary>
/// <param name="pID"></param>
Thread::Thread()
	:mTask(nullptr), mNeedsTask(true)
{
	Start();
}

/// <summary>
/// 
/// </summary>
Thread::~Thread()
{
}

/// <summary>
/// 
/// </summary>
void Thread::Start()
{
	mThread = std::thread(threadMain, this);
}

/// <summary>
/// 
/// </summary>
/// <param name="pTask"></param>
void Thread::SetTask(Task* pTask)
{
	mTask = pTask;
	mNeedsTask = false;
}

/// <summary>
/// 
/// </summary>
void Thread::Run()
{
	while (true)
	{
		if (mTask)
		{
			SetThreadAffinity(mTask->ThreadAffinity());
			mTask->Run();
			mTask = nullptr;
		}
		else
		{
			mNeedsTask = true;
		}
		Sleep(1);
	}
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool Thread::NeedsTask()
{
	return mNeedsTask;;
}

/// <summary>
/// 
/// </summary>
/// <param name="pCore"></param>
void Thread::SetThreadAffinity(const int pCore)
{
	DWORD_PTR mask = (1 << pCore);
	SetThreadAffinityMask(mThread.native_handle(), mask);
}
