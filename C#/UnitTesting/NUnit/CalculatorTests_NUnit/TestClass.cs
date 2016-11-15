using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CalculatorNS;

namespace CalculatorTests_NUnit
{
	[TestFixture]
	public class TestClass
	{
		[Test]
		public void CalculatorCreate()
		{
			Calculator calc = new Calculator();
			Assert.IsNotNull(calc);
		}

		[Test]
		public void Calculator_BasicComputation()
		{
			double[] resultsAdd = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };
			double[] resultsSub = { -10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10};

			Calculator calc = new Calculator();			

			for (int i = 0; i <= 10; ++i)
			{
				Assert.AreEqual(resultsAdd[i], calc.Add(i, i));
			}

			for(int i = 0, j = 10; i <= 10; ++i, --j)
			{
				Assert.AreEqual(resultsSub[i], calc.Sub(i, j));
			}
		}

		[Test]
		public void Calculator_DivisionByZeroException()
		{
			Calculator calc = new Calculator();
			Assert.Throws<DivideByZeroException>(() => calc.Div(1, 0));
			Assert.Throws(typeof(DivideByZeroException), () => calc.Div(1, 0));
			Assert.DoesNotThrow(() => calc.Div(1, 1));
		}
	}
}
