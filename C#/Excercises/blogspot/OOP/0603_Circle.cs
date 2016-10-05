using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP
{
	namespace _0603
	{
		class Circle
			: Shape
		{
			protected double m_radius;

			public double Radius
			{
				get { return m_radius; }
				set { m_radius = value; }
			}

			public override double Perimeter()
			{
				return 3.14 * m_radius;
			}
		}
	}
}
