using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP
{
	namespace _0603
	{
		class Location
		{
			protected double m_x;
			protected double m_y;

			public double X
			{
				get { return m_x; }
				set { m_x = value; }
			}

			public double Y
			{
				get { return m_y; }
				set { m_y = value; }
			}
		}
	}
}
