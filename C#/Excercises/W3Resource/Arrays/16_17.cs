using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _16_17
	{
		public static void process()
		{
			int[] values = { 1, 9, 5, 6, 8, 7, 9, 8, 13, 3, 6, 5, 4 };
			secondSmallest(values);
			secondLargest(values);
		}

		private static void secondLargest(int[] values)
		{
			int biggest = Int32.MinValue;
			int secondBiggest = Int32.MinValue;
			foreach(int value in values)
			{
				if(biggest < value)
				{
					biggest = value;
				}
			}
			foreach(int value in values)
			{
				if(
					(value < biggest) &&
					(value > secondBiggest))
				{
					secondBiggest = value;
				}
			}
			Console.WriteLine("Second biggest: {0}", secondBiggest);
		}

		private static void secondSmallest(int[] values)
		{
			int smallest = Int32.MaxValue;
			int secondSmallest = Int32.MaxValue;
			foreach(int value in values)
			{
				if(smallest > value)
				{
					smallest = value;
				}
			}

			foreach(int value in values)
			{
				if(
					(value > smallest) &&
					(secondSmallest > value))
				{
					secondSmallest = value;
				}
			}
			Console.WriteLine("Second smallest: {0}", secondSmallest);
		}
	}
}
