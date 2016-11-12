using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sorts
{
	class _01
	{
		public static void process()
		{
			int[] values = { 10, 9, 8, 7, 6, 28, 5, 4, 3, 2, 1, 0 };
			for (int k = 0; k < values.Length; ++k)
			{
				for(int i = k; i < values.Length; ++i)
				{
					int temp = values[i];
					int j = 0;
					for (j = i; j >= k && values[j - k] > temp; j -= k)
					{
						values[j] = values[j - k];
					}
					values[j] = temp;

					/*foreach (int value in values)
					{
						Console.Write("{0} ", value);
					}
					Console.WriteLine();*/
				}
			}

			foreach (int value in values)
			{
				Console.Write("{0} ", value);
			}
			Console.WriteLine();
		}
	}
}
