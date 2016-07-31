using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Exceptions
{
	class Program
	{
		static void Main(string[] args)
		{
			try
			{
				string content = File.ReadAllText(@"D:\temp\wtf.txt");
				Console.WriteLine("Reading...");				
			}
			catch (IOException e)
			{
				Console.WriteLine("Catching...");
				Console.WriteLine(e.Message);
			}
			finally
			{
				Console.WriteLine("Cleaning...");
			}

			Console.WriteLine("Ending...");
			Console.ReadLine();
		}
	}
}
