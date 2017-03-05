using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _60_MinMaxArray
{
	public class ArrayFinder
	{
		static public void MinMaxIterative(float[] array, ref float min, ref float max)
		{
			min = float.MaxValue;
			max = float.MinValue;

			foreach (float item in array)
			{
				if (item < min) min = item;
				if (item > max) max = item;
			}			
		}

		static public void MinMax(float[] array, ref float min, ref float max)
		{
			min = array.Min();
			max = array.Max();
		}
	}
}
