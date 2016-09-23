using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _42
	{
		public static void process(
			)
		{
			string binNumber = "1000000";
			int power = 1;
			int result = 0;
			for(int round = binNumber.Length - 1; round >= 0; --round, power *= 2)
			{
				if(binNumber[round] == '1')
				{
					result += power;
				}
			}
			Console.WriteLine("{0}", result);
		}
	}
}
