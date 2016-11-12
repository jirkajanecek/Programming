using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _13
	{
		public static void process()
		{
			int[] values = { 1, 2, 3, 4, 5, 6, 7, 9 };
			int valueToAdd = 10;

			//unsorted variant
			List<int> unsorted = new List<int>(values);
			for(int i = 0; i <= unsorted.Count; ++i)
			{
				if(
					(i == unsorted.Count) ||
					(unsorted[i] > valueToAdd))
				{
					unsorted.Insert(i, valueToAdd);
					break;
				}
			}

			Console.Write("Unsorted: ");
			foreach (int value in unsorted)
			{
				Console.Write("{0} ", value);
			}
			Console.WriteLine();			

			SortedList<int, int> sortedList = new SortedList<int, int>();
			foreach(int value in values)
			{
				sortedList.Add(value, value);
			}
			sortedList.Add(valueToAdd, valueToAdd);

			Console.Write("Sorted: ");
			foreach (KeyValuePair<int,int> value in sortedList)
			{
				Console.Write("{0} ", value.Value);
			}
			Console.WriteLine();

		}
	}
}
