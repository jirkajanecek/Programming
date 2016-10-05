using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP
{
	namespace _0603
	{
		class Shape
		{
			protected Location c;

			public Shape()
			{
				c = new Location();
				c.X = 0;
				c.Y = 0;
			}

			public override string ToString()
			{
				return "General shape";
			}

			public virtual double Area()
			{
				return 0.0;
			}

			public virtual double Perimeter()
			{
				return 0.0;
			}
		}
	}
}
