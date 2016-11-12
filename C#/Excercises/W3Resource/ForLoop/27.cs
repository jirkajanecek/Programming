using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _27
	{
		public static void process(
			)
		{
			int number = 6;
			List<int> divisors = null;
			getPositiveDivisors(number, ref divisors);

			Console.Write("The positive divisors: ");
			foreach (int divisor in divisors)
			{
				Console.Write("{0} ", divisor);
			}
			Console.WriteLine();

			int sum = divisors.Sum();
			Console.WriteLine("The sum of the divisors is: {0}", sum);

			if (sum == number)
			{
				Console.WriteLine("It is pefect number");
			}
			else
			{
				Console.WriteLine("It is not perfect number");
			}
		}

		private static void getPositiveDivisors(
			int				number,
			ref List<int>	divisors
			)
		{
			divisors = new List<int>();
			for (int current = 1; current < number; ++current)
			{
				if (number % current == 0)
				{
					divisors.Add(current);
				}
			}
		}
	}
}
