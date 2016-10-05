using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP
{
	namespace _0605
	{
		class Square
			: _0603.Shape
		{

			double m_side = 0;

			public Square(
				double x,
				double y,
				double side
				)
			{
				base.c.X = x;
				base.c.Y = y;
				m_side = side;
			}


			public void Move(
				double x,
				double y
				)
			{
				base.c.X = x;
				base.c.Y = y;
			}


			public void scale(
				int factor
				)
			{
				m_side *= factor;
			}

			public override string ToString(
				)
			{
				return "Corner (" + base.c.X.ToString() + "," + base.c.Y.ToString() +"), side " + m_side.ToString();
			}


			public override double Area(
				)
			{
				return m_side * m_side;
			}
		}
	}
}
