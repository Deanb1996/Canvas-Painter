#pragma once
#include <functional>

class Task
{
private:
	std::function<void(void* param1, void* param2)> mFunction = nullptr;
	void* mParam1;
	void* mParam2;
	int mAffinity;
	bool mIsDone;

public:
	//Structors
	Task(std::function<void(void* param1, void* param2)> pFunction, void* pParam1, void* pParam2, const int pThreadAffinity);
	~Task();

	void Run();
	int ThreadAffinity();
	bool IsDone();
	void CleanUpTask();
};