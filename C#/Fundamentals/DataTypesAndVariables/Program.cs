using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataTypesAndVariables
{
	class Program
	{
		static void Main(string[] args)
		{
			int x = 7;
			int y = x + 3;
			Console.WriteLine(y);

			readingFromConsole();

			Console.ReadLine();
			return;
		}

		static void readingFromConsole(			
			)
		{
			Console.Write("Given me anything: ");
			String anything = Console.ReadLine();

			String answer = "You give me: " + anything;
			Console.WriteLine(answer);

			return;
		}
	}
}
