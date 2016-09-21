using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _16
	{
		public static void process(
			)
		{
			int numberOfItems = 5;
			List<int> result = new List<int>();
			
			for(
				int hits = 0, currentNumber = 1;
				(hits < numberOfItems) && (currentNumber < Int32.MaxValue);
				++currentNumber)
			{
				if(currentNumber % 2 == 0)
				{
					hits++;
					result.Add(currentNumber);
				}
			}

			foreach(int item in result)
			{
				Console.Write("{0} ", item);
			}
			Console.WriteLine();
		}
	}
}
