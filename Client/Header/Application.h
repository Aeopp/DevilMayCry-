#ifndef __APPLICATION_H__
#define __APPLICATION_H__
class Application final
{
public:
	Application();
	~Application() = default;
public:
	HRESULT ReadyApplication();
	HRESULT UpdateApplication();
	HRESULT ReleaseApplication();
};
#endif // !__APPLICATION_H__