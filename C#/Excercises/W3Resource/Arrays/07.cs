using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _07
	{
		public static void process(
			)
		{
			int[] secondArray = { 1, 5, 6, 7, 8, 9, 10, 11, 12 };
			int[] firstArray = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			int newArraySize = firstArray.Length + secondArray.Length;
			Debug.Assert(firstArray.Length == secondArray.Length);
			int[] result = new int[newArraySize];
			for(int i = 0, first = 0, second = 0; i < newArraySize; ++i)
			{
				if(first == firstArray.Length)
				{
					result[i] = secondArray[second];
					second++;
				}
				else if(second == secondArray.Length)
				{
					result[i] = firstArray[first];
					first++;
				}
				else if (firstArray[first] <= secondArray[second])
				{
					result[i] = firstArray[first];
					first++;
				}
				else
				{
					result[i] = secondArray[second];
					second++;
				}
			}

			foreach(int item in result)
			{
				Console.Write("{0} ", item);
			}
			Console.WriteLine();
		}
	}
}
