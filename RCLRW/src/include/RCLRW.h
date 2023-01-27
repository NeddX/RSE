#ifndef RCLRW_H
#define RCLRW_H

#include <msclr\marshal_cppstd.h>
#include <sdafx.h>

using namespace System;

namespace Advres
{
	// Forward declerations
	namespace RSE
	{
		class RSECore;
	}

	namespace RSE::Net
	{
		public ref class RSEFramework : public IDisposable
		{
		protected:
			RSE::RSECore* m_CoreClass;
			bool m_Disposed;
			bool m_Initialized;

		public:
			RSEFramework();
			~RSEFramework();
			!RSEFramework();
		
		public:
			void Init(String^ title, int width, int height);

		protected:
			virtual void Start();
			virtual void Update(float deltaTime);
		};
	}
}

#endif // RCLRW_H