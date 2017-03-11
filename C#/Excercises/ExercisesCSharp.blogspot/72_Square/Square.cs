using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _70_Shapes;

namespace _72_Square
{
	class Square : Rectangle
	{
		public Square(double side)
			: base(side, side)
		{
		}

		public Square(int x, int y, double side)
			: base(x, y, side, side)
		{
		}

		public override string ToString()
		{
			return base.ToString().Replace("Rectangle", "Square");
		}

		public void Scale()
		{
			_side1 *= 2;
			_side2 *= 2;
		}

		public void Move(int x, int y)
		{
			_location.X = x;
			_location.Y = y;
		}
	}
}
