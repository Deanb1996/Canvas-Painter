#include "Task.h"

/// <summary>
/// 
/// </summary>
/// <param name="mFunction"></param>
Task::Task(void(*pFunction)(), const int pThreadAffinity)
	:mFunction(pFunction), mAffinity(pThreadAffinity)
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
	mFunction();
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
