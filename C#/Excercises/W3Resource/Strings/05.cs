using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Strings
{
	class _05
	{
		public static void process()
		{
			string payload = "Test string more words";
			string[] stringArray = payload.Split(' ');
			Console.WriteLine("String contains {0} words.", stringArray.Length);
		}
	}
}
