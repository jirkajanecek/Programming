using System;

namespace _70_Shapes
{
	class Circle : Shape
	{
		private double _diameter;

		public double Diameter
		{
			get { return _diameter; }
		}

		public Circle(double diameter)
			: this(0,0,diameter)
		{
		}

		public Circle(int x, int y, double diameter) 
			: base(x, y)
		{
			if(diameter <= 0)
			{
				throw new ArgumentException();
			}

			_diameter = diameter;
		}

		public override double Area()
		{
			return Math.PI * (_diameter / 2);
		}

		public override double Perimeter()
		{
			return 2 * Math.PI * (_diameter / 2);
		}
	}
}
