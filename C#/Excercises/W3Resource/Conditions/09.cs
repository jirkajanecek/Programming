using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Conditions
{
	class _09
	{
		public static void process(
			)
		{
			int x = 0, y = 0;
			StringBuilder result = new StringBuilder();

			if (x == 0 && y == 0)
			{
				result.Append("In the middle of graph");
			}
			else if ((x == 0) || (y == 0))
			{
				result.Append("Inconclusive - somewhere on axe");
			}
			else if ((x > 0) && (y > 0))
			{
				result.Append("First quadrant");
			}
			else if ((x > 0) && (y < 0))
			{
				result.Append("Second quadrant");
			}
			else if ((x < 0) && (y < 0))
			{
				result.Append("Third quadrant");
			}
			else
			{
				result.Append("Fourth quadrant");
			}

			Console.WriteLine("The coordinate point ({0},{1}) lies in {2}", x, y, result.ToString());
		}
	}
}
