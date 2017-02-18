using System;

namespace Classes
{
	class Abstract
	{
		abstract class AbstractClass
		{
			private int m_property;
			public int Property
			{
				get { return m_property; }
				set { m_property = value; }
			}

			public abstract void method();

			public void methodWithBody()
			{
				Console.WriteLine("BaseClass::methodWithBody()");
			}
		}


		class DerivedClass : AbstractClass
		{
			public override void method()
			{
				Console.WriteLine("DerivedClass::method()");
			}
		}

		static public void process()
		{
			Console.WriteLine("----ABSTRACT----");			
			
			//abstract class can not be instantiated
			//BaseClass bc = new BaseClass();

			DerivedClass dc = new DerivedClass();
			dc.method();
			dc.methodWithBody();
			dc.Property = 5;

			Console.WriteLine("{0}", dc.Property);
		}
	}
}