using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _41
	{
		public static void process(
			)
		{
			int decNumber = 25;
			int power = 1;
			int rounds = 0;
			for (; power <= decNumber; rounds++, power *= 2){}
			power /= 2;
			int tmpNumber = decNumber;
			for(int round = 0; round < rounds; ++round, power /= 2)
			{
				Console.Write(tmpNumber / power);
				tmpNumber = tmpNumber % power;
			}
			Console.WriteLine();
		}
	}
}
