#pragma once
#include <thread>
#include "windows.h"
#include <iostream>
#include "Task.h"
#include <queue>

class Thread
{
private:
	std::thread mThread;
	Task* mTask;
	bool mNeedsTask;

	void SetThreadAffinity(const int pCore);
public:
	//Structors
	Thread();
	~Thread();

	void Start();
	void SetTask(Task* pTask);
	void Run();
	bool NeedsTask();
};
