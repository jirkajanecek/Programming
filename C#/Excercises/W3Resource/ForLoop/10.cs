using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _10
	{
		public static void process()
		{
			int rowMaxNumber = 1;
			for (int row = 0; row < 4; ++row, ++rowMaxNumber)
			{
				for(int collumn = 1; collumn <= rowMaxNumber; ++collumn)
				{
					Console.Write(collumn);
				}
				Console.WriteLine();
			}
		}
	}
}
