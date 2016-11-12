using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP
{
	class _0603_Test
	{
		public static void process()
		{
			_0603.Rectangle rectangle = new _0603.Rectangle();
			rectangle.Side1 = 5;
			rectangle.Side2 = 4;
			Debug.Assert(rectangle.Area() == 20 && rectangle.Perimeter() == 0);

			_0603.Circle circle = new _0603.Circle();
			circle.Radius = 100;
			Debug.Assert(circle.Area() == 0 && circle.Perimeter() == 314);
		}
	}
}
