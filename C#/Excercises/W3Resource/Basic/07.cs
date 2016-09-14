using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Basic
{
	class _07
	{
		public static void process(
			bool automatic = true
			)
		{
			int first = 25, second = 4;

			if (automatic)
			{
				Console.WriteLine("Input the first number: 25");
				Console.WriteLine("Input the second number: 4");
			}
			else
			{
				Console.WriteLine("Input the first number: ");
				first = Convert.ToInt32(Console.ReadLine());
				Console.WriteLine("Input the second number: ");
				second = Convert.ToInt32(Console.ReadLine());
			}

			Console.WriteLine("{0} + {1} = {2}", first, second, first + second);
			Console.WriteLine("{0} - {1} = {2}", first, second, first - second);
			Console.WriteLine("{0} * {1} = {2}", first, second, first * second);
			Console.WriteLine("{0} / {1} = {2}", first, second, first / second);
			Console.WriteLine("{0} mod {1} = {2}", first, second, first % second);
		}
	}
}
