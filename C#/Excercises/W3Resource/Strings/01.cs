using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Strings
{
	class _01
	{
		public static void process()
		{
			int i = 0;
			string payload = "Test string";
			foreach(char ch in payload)
			{
				++i;
			}
			Console.WriteLine("String size: {0}", i);
		}
	}
}
