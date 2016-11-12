using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _09
	{
		public static void process()
		{
			int[] values = { 5, 4, 3, 2, 1, 10, 9, 8, 7, 6 };
			int max = Int32.MinValue;
			int min = Int32.MaxValue;
			foreach(int value in values)
			{
				if(value >= max)
				{
					max = value;
				}
				if(value <= min)
				{
					min = value;
				}
			}
			Console.WriteLine("max is: {0} and min is {1}", max, min);
		}
	}
}
