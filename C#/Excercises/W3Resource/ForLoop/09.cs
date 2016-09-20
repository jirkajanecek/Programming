using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _09
	{
		public static void process()
		{
			int collumn = 1;
			for (int row = 0; row < 4; ++row, ++collumn)
			{
				for(int j = 0; j < collumn; ++j)
				{
					Console.Write("*");
				}
				Console.WriteLine();
			}
		}
	}
}
