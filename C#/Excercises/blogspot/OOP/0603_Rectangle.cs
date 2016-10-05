using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP
{
	namespace _0603
	{
		class Rectangle 
			: Shape
		{
			protected double m_side1;
			protected double m_side2;

			public double Side1
			{
				get { return m_side1; }
				set { m_side1 = value; }
			}

			public double Side2
			{
				get { return m_side2; }
				set { m_side2 = value; }
			}

			public override string ToString()
			{
				return "Rectangle";
			}

			public override double Area()
			{
				return m_side1 * m_side2;
			}
		}
	}	
}
