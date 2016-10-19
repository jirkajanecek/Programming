using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Structs
{
	class Program
	{
		struct TestStruct
		{
			public int one;
			public int two;

			public TestStruct(
				int givenOne,
				int givenTwo)
			{
				one = givenOne;
				two = givenTwo;
			}
		}

		static void Main(string[] args)
		{
			TestStruct test01 = new TestStruct(1,2);
			TestStruct test02 = new TestStruct();
			test02 = test01;

			TestStruct test03;
			test03.one = 1;
			test03.two = 2;
			test02 = test03;

			Console.ReadKey();
		}
	}
}
