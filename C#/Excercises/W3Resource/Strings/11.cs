using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Strings
{
	class _11
	{
		public static void process()
		{
			string payload = "Test string";
			char[] unsorted = payload.ToCharArray();
			char[] sorted = unsorted.OrderBy(c => c).ToArray();
			foreach(char ch in sorted)
			{
				Console.Write("{0} ", ch);
			}
			Console.WriteLine();
		}
	}
}
