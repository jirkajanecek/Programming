using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0701
	{
		class Table
		{
			protected int m_height;
			protected int m_width;
			protected string m_type = "Table";
			protected _0706.Leg[] m_legs = new _0706.Leg[4];

			public Table(
				int width,
				int height
				)
			{
				m_width = width;
				m_height = height;
				for( int i = 0; i < m_legs.Length; ++i)
				{
					m_legs[i] = new _0706.Leg();
				}
			}

			public int Height
			{
				get { return m_height; }
				set { m_height = value; }
			}

			public int Width
			{
				get { return m_width; }
				set { m_width = value; }
			}


			public void ShowData(
				)
			{
				Console.WriteLine("{2} Width: {0} Height: {1}", m_width, m_height, m_type);
				foreach (_0706.Leg leg in m_legs)
				{
					leg.ShowData();
				}
			}
		}
	}
}
