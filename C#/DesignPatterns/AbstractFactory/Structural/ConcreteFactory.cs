using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AbstractFactory.Structural
{

	/// <summary>
	/// Concreate implementation of abstract factory
	/// </summary>
	public class ConcreteFactory1 : AbstractFactory
	{
		/// <summary>
		/// Concrete factory creates concrete products
		/// </summary>
		/// <returns>Concrete product</returns>
		public override AbstractProductA CreateProductA()
		{
			return new ProductA1();
		}

		/// <summary>
		/// Concrete factory creates concrete products
		/// </summary>
		/// <returns>Concrete product</returns>
		public override AbstractProductB CreateProductB()
		{
			return new ProductB1();
		}
	}

	/// <summary>
	/// Concreate implementation of abstract factory
	/// </summary>
	public class ConcreteFactory2 : AbstractFactory
	{
		/// <summary>
		/// Concrete factory creates concrete products
		/// </summary>
		/// <returns>Concrete product</returns>
		public override AbstractProductA CreateProductA()
		{
			return new ProductA2();
		}

		/// <summary>
		/// Concrete factory creates concrete products
		/// </summary>
		/// <returns>Concrete product</returns>
		public override AbstractProductB CreateProductB()
		{
			return new ProductB2();
		}
	}
}
