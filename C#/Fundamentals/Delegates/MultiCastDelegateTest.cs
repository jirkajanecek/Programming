using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Delegates
{
	class MulticastDelegateTest
	{
		private delegate void PrintDelegate();

		public static void processTest()
		{
			PrintDelegate forward = printOne;
			forward += printTwo;
			printList(forward.GetInvocationList());
			Console.WriteLine("---");

			PrintDelegate backward = printTwo;
			backward += printOne;
			printList(backward.GetInvocationList());
			Console.WriteLine("---");

			forward();
			Console.WriteLine("---");
			backward();
		}

		static void printList(
			Delegate[] list)
		{
			foreach (Delegate item in list)
			{
				Console.WriteLine("Method name in invocation list:" + item.Method.Name);
			}
		}

		static void printOne()
		{
			Console.WriteLine("One");			
		}

		static void printTwo()
		{
			Console.WriteLine("Two");
		}
	}
}
