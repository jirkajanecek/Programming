using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Primes
{
	public class Primes
	{
		public static void processTests()
		{
			FindPrimesOnInterval();
			Factorization();
		}

		public static void FindPrimesOnInterval()
		{
			int from = 2;
			int to = 37;
			List<int> primes = new List<int>();
			
			for(int current = from; current <= to; ++current)
			{
				if(current <= 1)
				{
					continue;
				}

				bool hit = false;

				for(int tested = 2; tested < current; ++tested)
				{
					if(current % tested == 0)
					{
						hit = true;
						break;
					}
				}

				if (hit == false)
				{
					primes.Add(current);
				}
			}

			foreach(int prime in primes)
			{
				Console.Write("{0} ", prime);				
			}
			Console.WriteLine();
		}

		public static void Factorization()
		{
			int number = 420;
			int[] primes = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };
			int primeIndex = 0;

			while(true)
			{
				int remain = number % primes[primeIndex];
				if(remain != 0)
				{
					primeIndex++;
					continue;
				}
				Console.Write("{0} ",primes[primeIndex]);
				number = number / primes[primeIndex];
				if(number == 1)
				{
					break;
				}				
			}
			Console.WriteLine();
		}
	}
}
