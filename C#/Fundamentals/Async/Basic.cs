using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace Async
{
	class Basic
	{
		static Object sm_lock = new Object();

		public static async Task processTestAsync()
		{
			//calling async method
			Task<int> resultFromAsync = ReturnIntAsync(2, 3);

			SomeStuff();
			
			int result = await resultFromAsync;

			Console.WriteLine("Result from async: {0}", result);
			return;
		}

		static async Task<int> ReturnIntAsync(
			int param,
			int param2)
		{
			lock (sm_lock)
			{
				Console.WriteLine("Entering to Async method");
			}
			await Task.Delay(2000);
			lock (sm_lock)
			{
				Console.WriteLine("Exiting from Async method");
			}
			return param + param2;
		}

		public static void SomeStuff()
		{
			for (int i = 0; i < 2; ++i)
			{
				lock (sm_lock)
				{
					Console.WriteLine("Doing some work");					
				}
				Thread.Sleep(200);
			}
		}
	}
}
