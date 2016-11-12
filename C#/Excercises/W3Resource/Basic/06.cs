using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Basic
{
	class _06
	{
		public static void process(
			bool automatic = true)
		{
			int first = 0, second = 0, third = 0;
			if (automatic)
			{
				Console.WriteLine("Input the first number to multiply: 2");
				Console.WriteLine("Input the second number to multiply: 3");
				Console.WriteLine("Input the third number to multiply: 6");
				first = 2;
				second = 3;
				third = 6;
			}
			else
			{
				Console.Write("Input the first number to multiply: ");
				first = Convert.ToInt32(Console.ReadLine());
				Console.Write("Input the second number to multiply: ");
				second = Convert.ToInt32(Console.ReadLine());
				Console.Write("Input the third number to multiply: ");
				third = Convert.ToInt32(Console.ReadLine());
			}

			int result = first * second * third;
			Console.WriteLine("{0} x {1} x {2} = {3}", first, second, third, result);
		}
	}
}
