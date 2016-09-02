using System;
using System.Diagnostics;

namespace Classes
{
	class Interface
	{
		static public void process()
		{
			Console.WriteLine("----INTERFACE----");

			int result = 0;

			Class01 cl01 = new Class01();
			result = cl01.add(1, 2);
			Console.WriteLine("{0}", result);
			result = cl01.sub(1, 2);
			Console.WriteLine("{0}", result);

			Class02 cl02 = new Class02();
			result = cl02.add(1, 2);
			Console.WriteLine("{0}", result);
			result = cl02.sub(1, 2);
			Console.WriteLine("{0}", result);
		}
	}

	public interface IClass
	{
		int add(
			int a,
			int b
			);

		int sub(
			int a,
			int b
			);
	}


	class Class01 : IClass
	{
		public int add(
			int a,
			int b
			)
		{
			Console.WriteLine("Class01::add()");
			return a + b;
		}

		public int sub(
			int a,
			int b
			)
		{
			Console.WriteLine("Class01::sub()");
			return a - b;
		}
	}

	class Class02 : IClass
	{
		public int add(
			int a,
			int b
			)
		{
			Console.WriteLine("Class02::add()");
			return a - b;
		}

		public int sub(
			int a,
			int b
			)
		{
			Console.WriteLine("Class02::sub()");
			return a + b;
		}
	}
}