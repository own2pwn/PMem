#include "Driver.h"

using namespace sploit::memory;

Driver::Driver(const char* FileName, const char* ServiceName, const char* DisplayName):
	m_FileName(FileName),
	m_ServiceName(ServiceName),
	m_DisplayName(DisplayName)
{
	if (!this->LoadDriver())
		ExitProcess(0);
}

Driver::~Driver()
{
	this->UnloadDriver();
}

bool Driver::LoadDriver()
{
	if (!this->Installed())
	{
		if (!this->GetPath())
			return false;

		if (!this->InstallService())
			return false;

		if (!this->OpenDriver())
			return false;
	}
	return true;
}

bool Driver::OpenDriver()
{
	std::string ServiceName = m_ServiceName;
	std::string PathName = "\\\\.\\";
	m_Driver = CreateFileA(std::string(PathName + ServiceName).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (m_Driver == INVALID_HANDLE_VALUE)
		return false;

	return true;
}

bool Driver::InstallService()
{
	SC_HANDLE			hService;
	SC_HANDLE			hServiceMngr;

	this->UnloadDriver(); 

	hServiceMngr = OpenSCManagerA(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!hServiceMngr)
		return false;

	hService = CreateServiceA(hServiceMngr, m_ServiceName, m_DisplayName, SERVICE_START,
	SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, m_DriverPath, NULL, NULL, NULL, NULL, NULL);
	if (!hService)
	{
		CloseServiceHandle(hServiceMngr);
		return false;
	}
	CloseServiceHandle(hServiceMngr);

	if (!StartServiceA(hService, 0, NULL))
		return false;

	CloseServiceHandle(hService);
	return true;
}

void Driver::UnloadDriver()
{
	SC_HANDLE			hServiceMngr;
	SC_HANDLE			hService;
	SERVICE_STATUS		Status;
	HKEY				hKey;

	hServiceMngr = OpenSCManagerA(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!hServiceMngr)
		return;

	hService = OpenServiceA(hServiceMngr, m_ServiceName, SERVICE_ALL_ACCESS);
	if (!hService)
		return;

	CloseServiceHandle(hServiceMngr);


	ControlService(hService, SERVICE_CONTROL_STOP, &Status);
	DeleteService(hService);
	CloseServiceHandle(hService);


	RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\services\\", 0, KEY_ALL_ACCESS, &hKey);
	RegDeleteTreeA(hKey, m_ServiceName);
	RegCloseKey(hKey);
}

bool Driver::Installed()
{
	std::string ServiceName = m_ServiceName;
	std::string PathName = "\\\\.\\";
	m_Driver = CreateFileA(std::string(PathName + ServiceName).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(m_Driver == INVALID_HANDLE_VALUE)
		return false;

	return true;
}

bool Driver::GetPath()
{
	size_t psize;

	m_DriverPath = new char[MAX_PATH];
	GetFullPathNameA(m_FileName, MAX_PATH, m_DriverPath, NULL);
	psize = strlen(m_DriverPath);
	if (!psize)
	{
		delete m_DriverPath;
		return false;
	}
	return true;
}
