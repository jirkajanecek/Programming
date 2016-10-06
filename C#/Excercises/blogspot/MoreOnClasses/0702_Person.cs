using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0702
	{
		class Person
		{
			House m_house;
			string m_name;

			public House House
			{
				get { return m_house; }
				set { m_house = value; }
			}

			public string Name
			{
				get { return m_name; }
				set { m_name = value; }
			}

			public void ShowData()
			{
				Console.WriteLine("My name is: {0}", m_name);
				m_house.ShowData();
				m_house.GetDoor().ShowData();
			}
		}
	}
}
