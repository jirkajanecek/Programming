using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CalculatorNS
{
	class Program
	{
		static void Main(string[] args)
		{
			Calculator calc = new Calculator();
			Console.WriteLine(calc.Add(1, 1));
			Console.WriteLine(calc.Sub(1, 1));
			Console.WriteLine(calc.Mul(1, 1));
			Console.WriteLine(calc.Div(1, 1));
		}
	}
}
