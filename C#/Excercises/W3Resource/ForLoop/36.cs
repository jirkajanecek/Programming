using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _36
	{
		public static void process(
			)
		{
			int numberOfRows = 9;
			int initSpaces = numberOfRows - 1;
			int numbersInRow = 1;
			for(int row = 1; row <= numberOfRows; ++row, --initSpaces, numbersInRow += 2)
			{
				for(int space = 0; space < initSpaces; ++space)
				{
					Console.Write(" ");
				}
				int content = 1;
				for (int number = 0; number < numbersInRow; ++number)
				{
					Console.Write(content);
					if(number < (numbersInRow / 2))
					{
						content++;
					}
					else
					{
						content--;
					}
				}
				Console.WriteLine();
			}
			Console.WriteLine();
		}
	}
}
