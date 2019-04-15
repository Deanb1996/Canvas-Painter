#pragma once

class Task
{
private:
	void(*mFunction)() = nullptr;
	int mAffinity;
	bool mIsDone;

public:
	Task(void(*pFunction)(), const int pThreadAffinity);
	~Task();

	void Run();
	int ThreadAffinity();
	bool IsDone();
	void CleanUpTask();
};