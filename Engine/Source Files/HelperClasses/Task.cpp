#include "Task.h"

/// <summary>
/// 
/// </summary>
/// <param name="mFunction"></param>
Task::Task(std::function<void(void* param1, void* param2)> pFunction, void* pParam1, void* pParam2, const int pThreadAffinity)
	:mParam1(pParam1), mParam2(pParam2), mFunction(pFunction), mAffinity(pThreadAffinity)
{
}

/// <summary>
/// 
/// </summary>
Task::~Task()
{
}

/// <summary>
/// 
/// </summary>
void Task::Run()
{
	mFunction(mParam1, mParam2);
	mIsDone = true;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
int Task::ThreadAffinity()
{
	return mAffinity;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool Task::IsDone()
{
	return mIsDone;
}

/// <summary>
/// 
/// </summary>
void Task::CleanUpTask()
{
	delete this;
}
