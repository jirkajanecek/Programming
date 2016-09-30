using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sorts
{
	class _03
	{
		public static void process()
		{
			Console.WriteLine("***");

			int[] values = { 10, 9, 8, 7, 6, 28, 5, 4, 3, 2, 1, 0 };
			for(int i = 0; i < values.Length; ++i)
			{
				for(int j = i+1; j < values.Length; ++j)
				{
					if(values[i] > values[j])
					{
						int temp = values[i];
						values[i] = values[j];
						values[j] = temp;
					}

					foreach (int value in values)
					{
						Console.Write("{0} ", value);
					}
					Console.WriteLine();
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
