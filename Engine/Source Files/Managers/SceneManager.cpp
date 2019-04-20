#include "SceneManager.h"

/// <summary>
/// Updates the input and ecsManager every frame
/// Updates the scene every frame
/// Assigns the threads tasks every frame
/// Calculates all the timing of the scene
/// </summary>
void SceneManager::Update()
{
	mCurrentTime = std::chrono::high_resolution_clock::now();
	mDeltaTime = mCurrentTime - mPreviousTime;

	mThreadManager->ProcessTasks();
	mEcsManager->ProcessSystems();
	mInputManager->Update();
	mScene->Update();

	//mThreadManager->AddTask([](void*, void*) {OutputDebugString(L"'Ello m8"); }, nullptr, nullptr, 0);


	// Average the fps over n frames.
	mAverageDeltaTime = 0;
	for (auto i = 0; i < mLast50Frames.size() - 1; i++)
	{
		mLast50Frames[i] = mLast50Frames[i + 1];
	}
	mLast50Frames[mLast50Frames.size() - 1] = DeltaTime();

	for (auto i = 0; i < mLast50Frames.size(); i++)
	{
		mAverageDeltaTime += mLast50Frames[i];
	}
	mAverageDeltaTime = mAverageDeltaTime / mLast50Frames.size();

	mFps = static_cast<int>(1 / mAverageDeltaTime);

	mPreviousTime = mCurrentTime;
}

/// <summary>
/// Calculates delta time
/// </summary>
/// <returns>Delta time of the scene</returns>
const double SceneManager::DeltaTime() const
{
	return mDeltaTime.count() / pow(10, 9); // (or 1e+9)

}

/// <summary>
/// Calculates the total elapsed time of the scene
/// </summary>
/// <returns>Total elapsed time of the scene</returns>
const double SceneManager::Time() const
{
	auto time = mCurrentTime - mStartTime;
	return time.count() / pow(10, 9); // (or 1e+9)
}

/// <summary>
/// Get method for scenes fps
/// </summary>
/// <returns>Average fps of the scene</returns>
const int& SceneManager::Fps() const
{
	return mFps;
}

/// <summary>
/// Sets the window width and height inside the scene manager
/// </summary>
/// <param name="pWidth">Width of the window</param>
/// <param name="pHeight">Height of the window</param>
void SceneManager::SetWindowWidthHeight(const float & pWidth, const float & pHeight)
{
	mWindowWidth = pWidth;
	mWindowHeight = pHeight;
}

/// <summary>
/// Get method for window width
/// </summary>
/// <returns>Width of the window</returns>
const float & SceneManager::WindowWidth() const
{
	return mWindowWidth;
}

/// <summary>
/// Get method for window height
/// </summary>
/// <returns>Height of the window</returns>
const float & SceneManager::WindowHeight() const
{
	return mWindowHeight;
}

/// <summary>
/// Default constructor
/// </summary>
SceneManager::SceneManager()
{
}

/// <summary>
/// Default destructor
/// </summary>
SceneManager::~SceneManager()
{
}

/// <summary>
/// If an instance of the scene manager does not already exists, creates one and then provides a pointer to it
/// </summary>
/// <returns>Pointer to the scene manager instance</returns>
std::shared_ptr<SceneManager> SceneManager::Instance()
{
	static std::shared_ptr<SceneManager> instance{ new SceneManager };
	return instance;
}
