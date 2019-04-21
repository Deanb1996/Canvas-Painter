#include "Task.h"

/// <summary>
/// Constructs a Task class that encapsulates a std::function pointer, its parameters and the thread affinity for the task
/// </summary>
/// <param name="pFunction">Function pointer to the given function</param>
/// <param name="pParam1">The first parameter of the function</param>
/// <param name="pParam2">The second parameter of the function</param>
/// <param name="pThreadAffinity">Thread affinity of the task</param>
Task::Task(std::function<void(void* param1, void* param2)> pFunction, void* pParam1, void* pParam2, const std::vector<int>& pThreadAffinity)
	:mParam1(pParam1), mParam2(pParam2), mFunction(pFunction), mAffinity(pThreadAffinity), mIsDone(false)
{
}

/// <summary>
/// Default destructor
/// </summary>
Task::~Task()
{
}

/// <summary>
/// Run method of the task, runs the contained std::function pointer and sets isDone to true when complete
/// </summary>
void Task::Run()
{
	mFunction(mParam1, mParam2);
	mIsDone = true;
}

/// <summary>
/// Get method for the tasks thread affinity
/// </summary>
/// <returns>The thread affinity of the task</returns>
const std::vector<int>& Task::ThreadAffinity()
{
	return mAffinity;
}

/// <summary>
/// Get method for the tasks isDone status
/// </summary>
/// <returns>Tasks isDone status</returns>
bool Task::IsDone()
{
	return mIsDone;
}

/// <summary>
/// Cleans up the task from memory
/// </summary>
void Task::CleanUpTask()
{
	delete this;
}
