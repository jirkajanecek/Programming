using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _32
	{
		public static void process(
			)
		{
			bool otherDivisorExists = false;
			int examinedNumber = 17;
			if(examinedNumber != 1)
			{
				for (int divisor = 2; divisor < examinedNumber; ++divisor)
				{
					if (examinedNumber % divisor == 0)
					{
						otherDivisorExists = true;
						break;
					}
				}
			}

			if(
				(examinedNumber == 1) ||
				(otherDivisorExists == true))
			{
				Console.WriteLine("This is not prime.");
			}
			else
			{
				Console.WriteLine("This is prime.");
			}
		}
	}
}
