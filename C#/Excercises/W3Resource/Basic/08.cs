using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Basic
{
	class _08
	{
		public static void process(
			)
		{
			int number = 5;
			for(int i = 0; i < 10; ++i)
			{
				Console.WriteLine("{0} * {1} = {2}", number, i, number * i);
			}
		}
	}
}
