using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace ServiceLibrary
{
	// NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service1" in both code and config file together.
	public class Calculator : ICalculator
	{
		public double Add(
			double n1, 
			double n2)
		{
			double result = n1 + n2;
			PrintReport(n1, n2, result, "Add");
			return result;
		}

		public double Divide(
			double n1,
			double n2)
		{
			double result = n1 / n2;
			PrintReport(n1, n2, result, "Divide");
			return result;
		}

		public double Multiply(
			double n1,
			double n2)
		{
			double result = n1 * n2;
			PrintReport(n1, n2, result, "Multiply");
			return result;
		}

		public double Subtract(
			double n1,
			double n2)
		{
			double result = n1 - n2;
			PrintReport(n1, n2, result, "Subtract");
			return result;
		}

		private void PrintReport(
			double n1,
			double n2,
			double result,
			string methodName)
		{
			Console.WriteLine("Received {0}({1},{2})", methodName, n1, n2);
			Console.WriteLine("Returned: {0}", result);
		}
	}
}
