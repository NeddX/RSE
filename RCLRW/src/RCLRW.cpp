#include <RSE.h>

#include "include/RCLRW.h"

namespace Advres
{
	RSE::Net::RSEFramework::RSEFramework()
	{
		m_CoreClass = new RSE::RSECore();
		m_Disposed = false;
		m_Initialized = true;
	}

	RSE::Net::RSEFramework::~RSEFramework()
	{
		delete m_CoreClass;
		m_Disposed = true;
	}

	RSE::Net::RSEFramework::!RSEFramework()
	{
		this->~RSEFramework();
	}

	void RSE::Net::RSEFramework::Init(String^ title, int width, int height)
	{
		m_CoreClass->Init(msclr::interop::marshal_as<std::string>(title).c_str(), width, height);
	}

	void RSE::Net::RSEFramework::Start()
	{
		throw gcnew System::NotImplementedException();
	}

	void RSE::Net::RSEFramework::Update(float deltaTime)
	{
		throw gcnew System::NotImplementedException();
	}
}
