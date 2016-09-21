using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _21
	{
		public static void process(
			)
		{
			int items = 5;
			int sumSeries = 0;
			int needle = 9;
			for (int i = 0; i < items; ++i, needle = (needle * 10) + 9 )
			{
				Console.Write("{0} ", needle);
				sumSeries += needle;
			}
			Console.WriteLine();
			Console.WriteLine("The sum of the series = {0}", sumSeries);
		}
	}
}
