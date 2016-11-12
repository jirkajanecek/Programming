using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP
{
	class _0605_Test
	{
		public static void process()
		{
			_0605.Square square = new _0605.Square(5, 10, 10);
			Debug.Assert(square.Area() == 100);
		}
	}
}
