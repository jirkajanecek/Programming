using System;

namespace Classes
{
	class Inheritance
	{
		static public void process()
		{
			Console.WriteLine("\n------INHERITANCE------");

			BaseClass bc = new BaseClass();
			bc.baseMethod();

			DerivedClass dc = new DerivedClass();
			dc.baseMethod();
			dc.derivedMethod();
		}


		class BaseClass
		{
			public void baseMethod()
			{
				Console.WriteLine("BaseClass::baseMethod()");
			}
		};

		class DerivedClass : BaseClass
		{
			public void derivedMethod()
			{
				Console.WriteLine("DerivedClass::derivedMethod()");
			}
		};
	};
}