using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Conditions
{
	class _04
	{
		public static void process(
			bool automatic = true
			)
		{
			int year = 2016;
			if(!automatic)
			{
				Console.Write("Input year: ");
				year = Convert.ToInt32(Console.ReadLine());
			}

			bool leapYear = false;

			/*
				 To determine whether a year is a leap year, follow these steps:
					If the year is evenly divisible by 4, go to step 2. Otherwise, go to step 5.
					If the year is evenly divisible by 100, go to step 3. Otherwise, go to step 4.
					If the year is evenly divisible by 400, go to step 4. Otherwise, go to step 5.
					The year is a leap year (it has 366 days).
					The year is not a leap year (it has 365 days).
			 * */

			if (year % 4 == 0)
			{
				if (year % 100 == 0)
				{
					if(year % 400 == 0)
					{
						leapYear = true;
					}
				}
				else
				{
					leapYear = true;
				}
			}

			StringBuilder result = new StringBuilder(year.ToString());
			if (leapYear)
			{
				result.Append(" is ");
			}
			else
			{
				result.Append(" is not ");
			}
			Console.WriteLine("{0}leap year", result.ToString());
		}
	}
}
