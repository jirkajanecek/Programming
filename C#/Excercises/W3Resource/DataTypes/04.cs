using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataTypes
{
	class _04
	{
		public static void process(
			bool automatic = true
			)
		{
			int a = 20, b = 12;
			string operatorString = "-";

			if (!automatic)
			{
				Console.WriteLine("Input first number: ");
				a = Convert.ToInt32(Console.ReadLine());
				Console.WriteLine("Input second number: ");
				b = Convert.ToInt32(Console.ReadLine());
				Console.WriteLine("Input operation: ");
				operatorString = Console.ReadLine();
			}

			BinaryOperationDelegate operation = null;

			switch(operatorString)
			{
				case "-":
					operation = new BinaryOperationDelegate(sub);
					break;

				case "+":
					operation = new BinaryOperationDelegate(sum);
					break;

				case "*":
					operation = new BinaryOperationDelegate(mul);
					break;

				case "/":
					operation = new BinaryOperationDelegate(div);
					break;

				case "x":
					operation = new BinaryOperationDelegate(rem);
					break;

				default:					
					break;
			}

			int result = operation(a, b);
			Console.WriteLine("{0} {1} {2} = {3}", a, operatorString, b, result);
		}

		delegate int BinaryOperationDelegate(int a, int b);

		private static int sum(
			int a,
			int b
			)
		{
			return a + b;
		}

		private static int sub(
			int a,
			int b
			)
		{
			return a - b;
		}

		private static int mul(
			int a,
			int b
			)
		{
			return a * b;
		}

		private static int div(
			int a,
			int b
			)
		{
			return a / b;
		}

		private static int rem(
			int a,
			int b
			)
		{
			return a % b;
		}
	}
}
