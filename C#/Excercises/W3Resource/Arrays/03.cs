using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _03
	{
		public static void process(
			)
		{
			int[] values = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			int sum = 0;
			for(int i = 0; i < values.Length; ++i)
			{
				sum += values[i];
			}
			Console.WriteLine("Sum: {0}", sum);
		}
	}
}
