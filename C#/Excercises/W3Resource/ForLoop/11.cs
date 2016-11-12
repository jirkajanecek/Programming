using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _11
	{
		public static void process()
		{
			int printedNumber = 1;
			int maxCollumns = 1;
			for (int row = 0; row < 4; ++row, ++printedNumber, maxCollumns++)
			{
				for(int collumn = 0; collumn < maxCollumns; ++collumn)
				{
					Console.Write(printedNumber);
				}
				Console.WriteLine();
			}
		}
	}
}
