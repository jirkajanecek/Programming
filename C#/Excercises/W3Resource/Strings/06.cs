using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Strings
{
	class _06
	{
		public static void process()
		{
			string payload1 = "Test string";
			string payload2 = "Test string";
			bool result = compareTwoStrings(payload1, payload2);
			if(result)
			{
				Console.WriteLine("Strings are the same");
			}
			else
			{
				Console.WriteLine("Strings are different");
			}
		}


		private static int getLength(
			string payload
			)
		{
			int length = 0;
			foreach (char ch in payload)
			{
				length++;
			}
			return length;
		}

		private static bool compareTwoStrings(
			string string1,
			string string2
			)
		{
			int length1 = getLength(string1);
			int length2 = getLength(string2);
			if (length1 != length2)
			{
				return false;
			}

			for (int i = 0; i < length1; ++i)
			{
				if (string1[i] != string2[i])
				{
					return false;
				}
			}		

			return true;
		}
	}
}
