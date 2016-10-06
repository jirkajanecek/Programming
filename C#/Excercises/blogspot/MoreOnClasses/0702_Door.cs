using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0702
	{
		class Door
		{
			private string m_color;

			public string Color
			{
				get { return m_color; }
				set { m_color = value; }
			}

			public void ShowData()
			{
				Console.WriteLine("I am a door, my color is {0}", m_color);
			}
		}
	}
}
