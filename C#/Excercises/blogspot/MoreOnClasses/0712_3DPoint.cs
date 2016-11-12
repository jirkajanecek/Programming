using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0712
	{
		class Point3D
		{
			private double m_x;
			private double m_y;
			private double m_z;

			public double Z
			{
				get { return m_z; }
				set { m_z = value; }
			}


			public double Y
			{
				get { return m_y; }
				set { m_y = value; }
			}


			public double X
			{
				get { return m_x; }
				set { m_x = value; }
			}

			public Point3D(
				double x,
				double y,
				double z)
			{
				m_x = x;
				m_y = y;
				m_z = z;
			}

			public void MoveTo(
				double x,
				double y,
				double z)
			{
				m_x = x;
				m_y = y;
				m_z = z;
			}

			public double DistanceTo(
				Point3D other
				)
			{
				double sum = Math.Pow(m_x - other.X, 2);
				sum += Math.Pow(m_y - other.Y, 2);
				sum += Math.Pow(m_z - other.Z, 2);
				return Math.Sqrt(sum);
			}


			public new string ToString()
			{
				return "(" + m_x.ToString() + "," + m_y.ToString() + "," + m_z.ToString() + ")";
			}

		}
	}
}
