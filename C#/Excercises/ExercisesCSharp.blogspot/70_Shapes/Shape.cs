using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _70_Shapes
{
	abstract class Shape
	{
		protected Location _location = null;

		public Location Location
		{
			get{ return _location;}
		}

		public Shape() 
			: this(0,0)
		{			
		}

		public Shape(int x, int y)
		{
			_location = new Location(x, y);
		}

		public abstract double Area();

		public abstract double Perimeter();

		public override string ToString()
		{
			return $"Shape " + _location.ToString();
		}
	}
}
