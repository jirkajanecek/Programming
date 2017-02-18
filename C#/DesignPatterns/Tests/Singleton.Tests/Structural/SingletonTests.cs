using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;
using Singleton;


namespace Singleton.Tests
{
	[TestFixture]
	class SingletonTests
	{
		[Test]
		public void Singleton_Create()
		{
			var firstInstance = Singleton.Instance();
			var secondInstance = Singleton.Instance();

			Assert.AreSame(firstInstance, secondInstance, "singleton instrances are different");
		}
	}
}
