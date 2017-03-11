using System;

namespace _70_Shapes
{
	class Rectangle : Shape
	{
		private double _side1;
		private double _side2;

		public double Side1
		{
			get { return _side1; }
		}

		public double Side2
		{
			get { return _side2; }
		}

		public Rectangle(double side1, double side2)
			: this(0, 0, side1, side2)
		{
		}

		public Rectangle(int x, int y, double side1, double side2)
			: base(x, y)
		{
			if ((side1 <= 0) || (side2 <= 0))
			{
				throw new ArgumentException("Sides must be possitive");
			}

			_side1 = side1;
			_side2 = side2;
		}

		public override double Area()
		{
			return _side1 * _side2;
		}

		public override double Perimeter()
		{
			return 2 * (Side1 + Side2);
		}
	}
}
