using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _30
	{
		public static void process(
			)
		{
			int levels = 9;
			Debug.Assert(levels % 2 == 1);
			int initSpaces = levels / 2;
			int currentStars = 1;
			int middleLevel = levels / 2 + 1;
			for(int level = 1; level <= levels; ++level)
			{
				StringBuilder builder = new StringBuilder();
				builder.Append(' ', initSpaces);
				builder.Append('*', currentStars);
				Console.Write(builder.ToString());

				if (level < middleLevel)
				{
					initSpaces--;
					currentStars += 2;
				}
				else
				{
					initSpaces++;
					currentStars -= 2;
				}
				Console.WriteLine();
			}
		}
	}
}
