using System;
using System.IO;
using NUnit.Framework;
using AbstractFactory.Structural;

namespace AbstractFactory.Tests.Structural
{
	[TestFixture]
	class ClientTests
	{
		[Test]
		public void Run_Basic()
		{
			AbstractFactory.Structural.AbstractFactory factory1 = new ConcreteFactory1();
			var clientA = new Client(factory1);

			AbstractFactory.Structural.AbstractFactory factory2 = new ConcreteFactory2();
			var clientB = new Client(factory2);

			using (StringWriter sw = new StringWriter())
			{
				Console.SetOut(sw);

				clientA.Run();
				clientB.Run();

				var expected = string.Format(
					$"AbstractFactory.Structural.ProductB1 interacts with AbstractFactory.Structural.ProductA1{Environment.NewLine}" + 
					$"AbstractFactory.Structural.ProductB2 interacts with AbstractFactory.Structural.ProductA2{Environment.NewLine}");

				Assert.AreEqual(expected, sw.ToString(), "Content should be the same");				
			}
		}
	}
}
