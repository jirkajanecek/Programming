using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Strings
{
	class _04
	{
		public static void process()
		{
			string payload = "Test string";
			char[] reversed = payload.Reverse().ToArray();
			foreach(char ch in reversed)
			{
				Console.Write("{0} ", ch);
			}
			Console.WriteLine();
		}
	}
}
