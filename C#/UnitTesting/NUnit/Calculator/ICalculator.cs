using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CalculatorNS
{
	/// <summary>
	/// Basic operation for calculators
	/// </summary>
	interface ICalculator
	{
		/// <summary>
		/// Adds two numbers
		/// </summary>
		/// <param name="first"></param>
		/// <param name="second"></param>
		/// <returns></returns>
		double Add(
			double first,
			double second);

		/// <summary>
		/// Subtracts two numbers
		/// </summary>
		/// <param name="first"></param>
		/// <param name="second"></param>
		/// <returns></returns>
		double Sub(
			double first,
			double second);

		/// <summary>
		/// Multiplies two numbers
		/// </summary>
		/// <param name="first"></param>
		/// <param name="second"></param>
		/// <returns></returns>
		double Mul(
			double first,
			double second);

		/// <summary>
		/// Divides two numbers
		/// </summary>
		/// <param name="first"></param>
		/// <param name="second"></param>
		/// <returns></returns>
		double Div(
			double first,
			double second);
	}
}
