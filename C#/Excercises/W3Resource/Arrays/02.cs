using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _02
	{
		public static void process(
			)
		{
			int[] values = new int[] { 1, 2, 3 };
			for (int i = values.Length - 1; i >= 0; --i)
			{
				Console.Write("{0} ", values[i]);
			}
		}
	}
}
