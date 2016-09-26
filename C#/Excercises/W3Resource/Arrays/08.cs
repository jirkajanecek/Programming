using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _08
	{
		public static void process()
		{
			int[] values = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 3, 5, 7, 9, 1, 5, 9, 1, 9, 1 };
			Dictionary<int, int> dictionary = new Dictionary<int, int>();
			foreach( int value in values)
			{
				if(dictionary.ContainsKey(value))
				{
					dictionary[value] += 1;
				}
				else
				{
					dictionary.Add(value, 1);
				}
			}
			foreach(KeyValuePair<int,int> pair in dictionary)
			{
				Console.WriteLine("{0} - {1}", pair.Key, pair.Value);
			}
		}
	}
}
