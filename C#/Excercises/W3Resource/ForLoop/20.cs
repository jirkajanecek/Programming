using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _20
	{
		public static void process(
			)
		{
			int levels = 6;
			int initSpaces = levels - 1;
			int stars = 1;

			for(int level = 0; level < levels; ++level, stars += 2, initSpaces--)
			{
				for(int spaces = 0; spaces < initSpaces; spaces++)
				{
					Console.Write(' ');
				}
				for (int star = 0; star < stars; ++star)
				{
					Console.Write('*');
				}
				Console.WriteLine();
			}
		}
	}
}
