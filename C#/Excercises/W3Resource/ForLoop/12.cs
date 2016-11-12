using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _12
	{
		public static void process()
		{
			int maxCollumn = 1;
			int currentNumber = 1;
			for (int row = 0; row < 4; ++row, ++maxCollumn)
			{
				for(int collumn = 0; collumn < maxCollumn; ++collumn, ++currentNumber)
				{
					Console.Write("{0} ", currentNumber);
				}
				Console.WriteLine();
			}
		}
	}
}
