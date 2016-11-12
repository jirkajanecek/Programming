using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _01
{
	class Quantity
	{
		public int Value;
	}

	class Test2
	{
		public static void process()
		{
			Quantity q1 = new Quantity();
			q1.Value = 5;

			Console.WriteLine(q1.Value);

			Quantity q2 = q1;
			q2.Value = 6;

			AddOneToQuantity(q1);
			AddOneToQuantity(q2);

			Console.WriteLine(q1.Value);
			Console.WriteLine(q2.Value);
		}

		private static void AddOne(
			int value)
		{
			value = value + 1;
		}

		private static void AddOneToQuantity(
			Quantity quantity
			)
		{
			quantity.Value = quantity.Value + 1;
		}
	}
}
