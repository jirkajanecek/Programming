using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AbstractFactory.Structural
{
	/// <summary>
	/// Concrete implementation of product A
	/// </summary>
	class ProductA1 : AbstractProductA
	{
	}

	/// <summary>
	/// Concrete implementation of product A
	/// </summary>
	class ProductA2 : AbstractProductA
	{
	}

	/// <summary>
	/// Concrete implementation of product B
	/// </summary>
	class ProductB1 : AbstractProductB
	{
		public override void Interact(AbstractProductA product)
		{
			Console.WriteLine($"{this.GetType().ToString()} interacts with {product.GetType().ToString()}");
		}
	}

	/// <summary>
	/// Concrete implementation of product B
	/// </summary>
	class ProductB2 : AbstractProductB
	{
		public override void Interact(AbstractProductA product)
		{
			Console.WriteLine($"{this.GetType().ToString()} interacts with {product.GetType().ToString()}");
		}
	}

}
