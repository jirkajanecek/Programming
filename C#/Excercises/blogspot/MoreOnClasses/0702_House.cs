using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0702
	{
		class House
		{
			protected int m_area;
			protected Door m_door;

			public int Area
			{
				get { return m_area; }
				set { m_area = value; }
			}

			public Door Door
			{
				get { return m_door; }
				set { m_door = value; }
			}

			public Door GetDoor()
			{
				return m_door;
			}


			public void ShowData()
			{
				Console.WriteLine("I'm a house, my area is {0}", m_area);
			}



		}
	}
}
