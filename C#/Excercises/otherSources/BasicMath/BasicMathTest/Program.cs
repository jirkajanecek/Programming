using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BasicMathTest
{
	class Program
	{
		static void Main(string[] args)
		{
			Primes.Primes.processTests();
			gcd.GCD.ProcessTests();

			Console.ReadKey();
		}
	}
}
