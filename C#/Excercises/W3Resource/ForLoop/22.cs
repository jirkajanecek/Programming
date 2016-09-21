using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _22
	{
		public static void process(
			)
		{
			int levels = 5;
						
			for(int level = 1, charsInRow = 1; level <= levels; ++level, ++charsInRow)
			{
				int print = (level % 2);
				for (int charInRow = 0; charInRow < charsInRow; ++charInRow, ++print)
				{
					Console.Write(print % 2);
				}
				Console.WriteLine();
			}
		}
	}
}
