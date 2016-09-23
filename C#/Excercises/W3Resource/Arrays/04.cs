using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _04
	{
		public static void process(
			)
		{
			int[] valuesFrom = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			int[] valuesTo = new int[valuesFrom.Length];
			for(int i = 0; i < valuesTo.Length; ++i)
			{
				valuesTo[i] = valuesFrom[i];
			}			
		}
	}
}
