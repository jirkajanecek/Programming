using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _29
	{
		public static void process(
			)
		{
			int examinedNumber = 153;
			if(
				(examinedNumber < 100) ||
				(examinedNumber > 999))
			{
				Console.WriteLine("This is not Armstrong number");
				return;
			}

			int first = examinedNumber / 100;
			int second = (examinedNumber % 100) / 10;
			int third = examinedNumber % 10;

			int sum = first * first * first;
			sum += second * second * second;
			sum += third * third * third;

			if (sum == examinedNumber)
			{
				Console.WriteLine("This is Armstrong number");
			}
			else
			{
				Console.WriteLine("This is not Armstrong number");
			}
		}
	}
}
