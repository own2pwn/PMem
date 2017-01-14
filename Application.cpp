#include "Application.h"

using namespace sploit;

/*Close always by pressing F10 so application closes driver*/

Application::Application() :
	m_Driver(nullptr),
	m_Memory(nullptr)
{
	m_Driver = new memory::Driver("WinIo64.sys", "WINIO", "WINIO");
	m_Memory = new memory::Memory(m_Driver);





	std::thread thread1(App_Thread, this);
	thread1.join();
}

Application::~Application()
{
	delete m_Driver; 
	delete m_Memory;
}

int Application::App_Thread(Application * app)
{
	if (app->m_Driver->GetHandle() == INVALID_HANDLE_VALUE)
		return 0;

		

	DWORD		result;
	DWORD		newresult = 0;
	bool		status;
	while (!GetAsyncKeyState(VK_F10))
	{
		/*example*/




		newresult++;

		status = app->m_Memory->ReadPhysMem((PVOID)0x537, &result, sizeof(result));
		app->m_Memory->WritePhysMem((PVOID)0x537, &newresult, sizeof(newresult));

		printf("status: %d | read/write result: %d\n", status, result);
		Sleep(100);
	}

	
	return 1;
}