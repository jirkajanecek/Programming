using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _11
	{
		public static void process()
		{
			int[] values = { 1, 5, 6, 7, 9, 82, 1, 4, 53, 0, 1 };
			for(int i = 0; i < values.Length; ++i)
			{
				for(int j = i + 1; j < values.Length; j++)
				{
					if (values[i] > values[j])
					{
						int tmp = values[j];
						values[j] = values[i];
						values[i] = tmp;
					}
				}
			}

			foreach(int value in values)
			{
				Console.Write("{0} ", value);
			}
			Console.WriteLine();
		}
	}
}
