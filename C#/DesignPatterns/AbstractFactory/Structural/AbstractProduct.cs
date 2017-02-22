using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AbstractFactory.Structural
{
	/// <summary>
	/// Abstract product type A
	/// </summary>
	public abstract class AbstractProductA
	{
	}


	/// <summary>
	/// Abstract product type B. It is able to interact with abstract product A
	/// </summary>
	public abstract class AbstractProductB
	{
		public abstract void Interact(AbstractProductA product);
	}
}
