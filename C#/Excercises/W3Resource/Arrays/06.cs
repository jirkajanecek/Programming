using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _06
	{
		public static void process(
			)
		{
			int[] values = { 1, 3, 1, 4, 3, 5, 1, 6, 3, 4 };
			for(int i = 0; i < values.Length; ++i)
			{
				bool hit = false;
				for (int j = 0; j < values.Length; ++j)
				{
					if (
						(values[i] == values[j]) &&
						(i != j))
					{
						hit = true;
						break;
					}
				}
				if(!hit)
				{
					Console.Write("{0} ", values[i]);
				}
			}
		}
	}
}
