#ifndef __ENGINE_TYPEDEF_H__
#define __ENGINE_TYPEDEF_H__
#pragma region TSTRING

#ifdef UNICODE
#ifndef __TSTRING_DEFINED__
#define __TSTRING_DEFINED__
typedef std::wstring TSTRING;
#endif // !__TSTRING_DEFINED__
#else
#ifndef __TSTRING_DEFINED__
#define __TSTRING_DEFINED__
typedef std::string TSTRING;
#endif // !__TSTRING_DEFINED__
#endif

#pragma endregion
#endif // !__ENGINE_TYPEDEF_H__