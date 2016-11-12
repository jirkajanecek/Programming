using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace gcd
{
	public class GCD
	{
		public static void ProcessTests()
		{
			CalculateGCD();
		}

		public static void CalculateGCD()
		{
			int first = 420;
			int second = 288;
					
			while(second != 0)
			{
				Console.Write("{0}%{1}", first, second);

				int remain = first % second;
				first = second;
				second = remain;

				Console.WriteLine("={0}", remain);
			}

			Console.WriteLine(first);
		}
	}
}
