using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _28
	{
		public static void process(
			)
		{
			Console.Write("The Perfect numbers within the given range:");
			int startInterval = 1, endInterval = 50;
			for (int currentNumber = startInterval; currentNumber <= endInterval; ++currentNumber)
			{
				int divisorsSum = 0;
				for (int divisor = 1; divisor < currentNumber; ++divisor)
				{
					if(currentNumber % divisor == 0)
					{
						divisorsSum += divisor;
					}
				}

				if(divisorsSum == currentNumber)
				{
					Console.Write(" {0}", currentNumber);
				}
			}
			Console.WriteLine();
		}
	}
}
