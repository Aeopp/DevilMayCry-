#ifndef __ENGINE_FUNCTION_H__
#define __ENGINE_FUNCTION_H__
#pragma region DELETE_DYNAMIC_MEMORY

template <typename T>
void SafeDelete(T& ptr)
{
	if (nullptr != ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDeleteArray(T& ptr)
{
	if (nullptr != ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}
#pragma endregion

#pragma region MANAGE_REFERENCE_COUNT
template <typename T>
UINT SafeAddRef(T& ptr)
{
	UINT nRefCount = 0;

	if (nullptr != ptr)
		nRefCount = ptr->AddRef();

	return nRefCount;
}

template <typename T>
UINT SafeRelease(T& ptr)
{
	UINT nRefCount = 0;

	if (nullptr != ptr)
	{
		nRefCount = ptr->Release();
		ptr = nullptr;
	}

	return nRefCount;
}
#pragma endregion

#pragma region SMART_POINTER
template <typename T>
class Deleter
{
public:
	void operator()(T* ptr)
	{
		ptr->Free();
		delete ptr;
	}
};
#pragma endregion

#endif // !__ENGINE_FUNCTION_H__