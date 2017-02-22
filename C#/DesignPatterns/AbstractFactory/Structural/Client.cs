using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AbstractFactory.Structural
{
	public class Client
	{
		AbstractProductA _abstractProductA;
		AbstractProductB _abstractProductB;

		public Client(AbstractFactory factory)
		{
			_abstractProductA = factory.CreateProductA();
			_abstractProductB = factory.CreateProductB();
		}

		public void Run()
		{
			_abstractProductB.Interact(_abstractProductA);
		}
	}
}
