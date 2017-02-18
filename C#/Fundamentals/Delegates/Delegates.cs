using System;
using System.Diagnostics;

namespace Delegates
{
	class Delegates
	{
		delegate int BinaryOperationDelegate(int a, int b);

		public static int add(int a, int b)
		{
			return a + b;
		}

		public static int multiply(int a, int b)
		{
			return a * b;
		}

		public static void process()
		{
			int a = 10, b = 8;

			//delegates - single version
			BinaryOperationDelegate methodDelegate = null;
			methodDelegate = new BinaryOperationDelegate(add);
			int c = methodDelegate(a, b);
			methodDelegate = new BinaryOperationDelegate(multiply);
			c = methodDelegate(a, c);
			Debug.Assert(c == 180);
			Console.WriteLine("{0}", c);

			//multicasting delegates
			BinaryOperationDelegate binaryDelegate;
			binaryDelegate = new BinaryOperationDelegate(add);
			binaryDelegate += new BinaryOperationDelegate(multiply);
			int d = binaryDelegate(a, b);
			Debug.Assert(d == 0x50);    //function add is processed but result is destroyed
			Console.WriteLine("{0}", d);

			return;
		}




	}
}
