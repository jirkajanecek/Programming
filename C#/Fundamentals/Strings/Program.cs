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
			program.stringBuilder();

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

			//string number format
			//https://msdn.microsoft.com/en-us/library/dwhawy9k(v=vs.110).aspx

			//currency
			String currency = String.Format("{0:C}", 123.45);
			Console.WriteLine(currency);

			String bigNumber = String.Format("{0:N}", 1234567890);
			Console.WriteLine(bigNumber);

			String percentage = String.Format("{0:P}", .123);
			Console.WriteLine(percentage);

			String phoneNumber = String.Format("{0:### ### ###}", 606674936);
			Console.WriteLine(phoneNumber);

			string stringBorderedWithSpaces = " Some text... ";

			string substring01 = stringBorderedWithSpaces.Substring(6);
			Console.WriteLine(substring01);

			string substring02 = stringBorderedWithSpaces.Substring(6, 4);
			Console.WriteLine(substring02);

			string lengths = String.Format(
				"Length before {0} -- length after trimming: {1}", 
				stringBorderedWithSpaces.Length, stringBorderedWithSpaces.Trim().Length);
			Console.WriteLine(lengths);

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

		void stringBuilder(
			)
		{
			StringBuilder stringBuilder = new StringBuilder();
			for (int i = 0; i < 100; ++i)
			{
				stringBuilder.AppendFormat("--{0}", i.ToString());
			}
			Console.WriteLine(stringBuilder);

			return;
		}
	}
}
