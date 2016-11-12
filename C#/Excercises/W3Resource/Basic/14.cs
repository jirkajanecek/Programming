using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Basic
{
	class _14
	{
		public static void process(
			bool automatic = true
			)
		{
			int celsius = 30;
			if (automatic)
			{
				Console.WriteLine("Enter the amount of celsius: 30");
			}
			else
			{
				Console.WriteLine("Enter the amount of celsius: ");
				celsius = Convert.ToInt32(Console.ReadLine());
			}

			Console.WriteLine("Kelvin = {0}", c2k(celsius));
			Console.WriteLine("Fahrenheit = {0}", c2f(celsius));
		}

		private static double c2f(
			int celsius)
		{
			return (double)celsius * 9 / 5 + 32;
		}

		private static double c2k(
			int celsius)
		{
			return (double)celsius + 273.15;
		}
	}
}
