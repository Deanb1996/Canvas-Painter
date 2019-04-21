#pragma once
#include <functional>
#include <vector>

class Task
{
private:
	std::function<void(void* param1, void* param2)> mFunction = nullptr;
	void* mParam1;
	void* mParam2;
	std::vector<int> mAffinity;
	bool mIsDone;

public:
	//Structors
	Task(std::function<void(void* param1, void* param2)> pFunction, void* pParam1, void* pParam2, const std::vector<int>& pThreadAffinity);
	~Task();

	void Run();
	const std::vector<int>& ThreadAffinity();
	bool IsDone();
	void CleanUpTask();
};