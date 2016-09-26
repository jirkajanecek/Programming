using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _10
	{
		static public void process()
		{
			List<int> odds = new List<int>();
			List<int> evens = new List<int>();
			int[] values = {0,1,2,3,4,5,6,7,8,9,10};
			foreach(int value in values)
			{
				if(value % 2 == 0)
				{
					evens.Add(value);
				}
				else
				{
					odds.Add(value);
				}
			}
			Console.Write("Evens: ");
			printList(evens);
			Console.WriteLine();
			Console.Write("Odds: ");
			printList(odds);
			Console.WriteLine();
		}


		private static void printList(List<int> values)
		{
			foreach (int value in values)
			{
				Console.Write("{0} ", value);
			}
		}
	}
}
