using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class _05
	{
		public static void process(
			)
		{
			int[] values = { 1, 3, 1, 4, 3, 5, 1, 6, 3, 4 };
			int duplicates = 0;

			//slower variant
			for(int i = 0; i < values.Length; ++i)
			{
				bool alreadyCounted = false;
				for (int k = 0; k < i; k++)
				{
					if(values[i] == values[k])
					{
						alreadyCounted = true;
						break;
					}
				}
				if(alreadyCounted)
				{
					continue;
				}
				for (int j = i+1; j < values.Length; ++j)
				{
					if(values[i] == values[j])
					{
						duplicates++;
					}
				}
			}

			//faster variant
			int duplicatesFaster = 0;
			for(int i = 0; i < values.Length; ++i)
			{
				for(int j = i+1; j < values.Length; ++j)
				{
					if(values[i] == values[j])
					{
						duplicatesFaster++;
						break;
					}
				}
			}
			Debug.Assert(duplicates == duplicatesFaster);
			Console.WriteLine(duplicates);
		}
	}
}
