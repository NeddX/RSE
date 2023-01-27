using System;
using Advres.RSE.Net;

namespace CSTest
{
	public class CSharpTest : RSEFramework
	{
		public CSharpTest()
		{
			// construct
		}

		protected override void Start()
		{
			Console.WriteLine("Started!");
			base.Start();
		}

		protected override void Update(float deltaTime)
		{
			base.Update(deltaTime);	
		}
	}

	internal class Program
	{
		static void Main(string[] args)
		{
			var core = new CSharpTest();
			core.Init("cool ass negga", 400, 400);
			Console.WriteLine("Hello, World!");
		}
	}
}