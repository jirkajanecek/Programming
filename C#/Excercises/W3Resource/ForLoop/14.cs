using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _14
	{
		public static void process(
			)
		{
			int pyramidLevels = 7;
			int itemsInRow = 1;
			int initSpaces = pyramidLevels - 1;

			for(int row = 0; row < pyramidLevels; ++row, --initSpaces, ++itemsInRow)
			{

				Console.Write(getSpaces(initSpaces));
				for (int items = 0; items < itemsInRow; ++items)
				{
					Console.Write("* ");
				}
				Console.WriteLine();
			}
		}

		private static string getSpaces(
			int			count
			)
		{
			return new string(' ', count);
		}
	}
}
