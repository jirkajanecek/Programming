using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Strings
{
	class Program
	{
		static void Main(string[] args)
		{
			Program program = new Program();
			program.concatenate();
			program.createDynamically();
			program.tips();

			Console.ReadLine();
		}

		void tips()
		{
			//string without backslash notation
			String stringWithoutBackslashCharacters = @"C:\";
			Console.WriteLine(stringWithoutBackslashCharacters);

			//basic fomating
			String formated = String.Format("Number: {0}, Strign {1}", 0, "Test");
			Console.WriteLine(formated);

			//currency
			String currency = String.Format("{0:C}", 123.45);
			Console.WriteLine(currency);

			String bigNumber = String.Format("{0:N}", 1234567890);
			Console.WriteLine(bigNumber);			

			return;
		}

		void createDynamically()
		{
			char[] payload = "Test string".ToCharArray();
			String dynamicString = new String(payload);
			Console.WriteLine(dynamicString);

			return;
		}

		void concatenate()
		{
			String oneString = "entire";
			String space = " ";
			String secondString = "string";

			String result01 = oneString + space + secondString;
			System.Console.WriteLine(result01);
			return;
		}
	}
}
