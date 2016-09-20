using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Conditions
{
	class _08
	{
		public static void process(
			bool automatic = true
			)
		{
			int first = 25, second = 63, third = 10;
			if(!automatic)
			{
				Console.Write("Input first number: ");
				first = Convert.ToInt32(Console.ReadLine());
				Console.Write("Input second number: ");
				second = Convert.ToInt32(Console.ReadLine());
				Console.Write("Input third number: ");
				third = Convert.ToInt32(Console.ReadLine());
			}

			StringBuilder result = new StringBuilder();

			if(
				(first >= second) && (first >= third))
			{
				result.Append("1st");
			}
			else if(second >= third)
			{
				result.Append("2nd");
			}
			else
			{
				result.Append("3rd");
			}

			Console.WriteLine("The {0} number od the gratest among others", result.ToString());
		}
	}
}
