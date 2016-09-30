using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Strings
{
	class _03
	{
		public static void process()
		{
			string payload = "Test string";
			char[] charArray = payload.ToCharArray();
			foreach(char ch in charArray)
			{
				Console.Write("{0} ", ch);
			}
			Console.WriteLine();
		}
	}
}
