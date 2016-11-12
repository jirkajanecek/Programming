using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _18
	{
		public static void process()
		{
			int rows = 10;
			int collumns = 10;
			int currentNumber = 0;
			int[][] values = new int[rows][];
			for(int row = 0; row < rows; row++)
			{
				values[row] = new int[collumns];
				for(int collumn = 0; collumn < collumns; collumn++, currentNumber++)
				{
					values[row][collumn] = currentNumber;
				}
			}

			foreach(int[] row in values)
			{
				foreach(int value in row)
				{
					if (value >= 10)
					{
						Console.Write("{0} ", value);
					}
					else
					{
						Console.Write(" {0} ", value);
					}
				}
				Console.WriteLine();
			}
		}
	}
}
