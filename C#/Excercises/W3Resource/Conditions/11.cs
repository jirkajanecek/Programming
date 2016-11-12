using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Conditions
{
	class _11
	{
		public static void process(
			bool automatic = true
			)
		{
			int a = 1, b = -1, c = -6;
			if(!automatic)
			{
				Console.Write("Input a: ");
				a = Convert.ToInt32(Console.ReadLine());
				Console.Write("Input b: ");
				b = Convert.ToInt32(Console.ReadLine());
				Console.Write("Input c: ");
				c = Convert.ToInt32(Console.ReadLine());
			}

			double discriminant = (b * b) - (4 * a * c);

			if(discriminant < 0)
			{
				Console.WriteLine("Equation has no real roots.");
				return;
			}
			else if(discriminant == 0)
			{
				double result = (b / (2 * a)) * (-1);
				Console.WriteLine("Equation has exactly one real root: {0}", result);
			}
			else
			{
				double resultA = (((-1) * (double)b) + Math.Sqrt(discriminant)) / (2 * a);
				double resultB = (((-1) * (double)b) - Math.Sqrt(discriminant)) / (2 * a);
				Console.WriteLine("Equation has two roots: {0} and {1}", resultA, resultB);
			}
		}
	}
}
