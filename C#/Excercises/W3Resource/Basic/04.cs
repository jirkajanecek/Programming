using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Basic
{
	class _04
	{
		public static void process()
		{
			/*
			 *	Write a C# Sharp program to print the result of the specified operations. Go to the editor
				Test data:
				• -1 + 4 * 6 
				• ( 35+ 5 ) % 7 
				• 14 + -4 * 6 / 11 
				• 2 + 15 / 6 * 1 - 7 % 2 
				Expected Output:
				 23
				 5
				 12
				 3
			*/

			Console.WriteLine("{0}", -1 + 4 * 6);
			Console.WriteLine("{0}", (35 + 5) % 7);
			Console.WriteLine("{0}", 14 + -4 * 6 / 11);
			Console.WriteLine("{0}", 2 + 15 / 6 * 1 - 7 % 2);
		}
	}
}
