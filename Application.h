#pragma once
#include <thread>
#include "Driver.h"
#include "Memory.h"

class Application
{
private:
	sploit::memory::Driver * m_Driver;
	sploit::memory::Memory * m_Memory;
public:
	Application();
	~Application();
private:
	int static App_Thread(Application * app);
};

