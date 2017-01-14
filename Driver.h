#pragma once
#include <windows.h>
#include <string>

namespace sploit{namespace memory{


class Driver
{
private:
	const char* m_FileName, *m_ServiceName, *m_DisplayName;
	char * m_DriverPath;

	HANDLE m_Driver;
public:
	Driver(const char* FileName = 0, const char* ServiceName = 0, const char* DisplayName = 0);
	~Driver();
	inline HANDLE GetHandle() { return m_Driver; }
private:
	bool LoadDriver();
	void UnloadDriver();
	bool Installed();
	bool InstallService();
	bool OpenDriver();
	bool GetPath();
};


}}