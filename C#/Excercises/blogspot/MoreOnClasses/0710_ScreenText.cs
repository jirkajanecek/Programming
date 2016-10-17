using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0710
	{
		class ScreenText
		{
			protected int m_x = 0;
			protected int m_y = 0;
			protected string m_text = "";

			public ScreenText(
				int x,
				int y,
				string text
				)
			{
				m_x = x;
				m_y = y;
				m_text = text;
			}

			public int X
			{
				get { return m_x; }
				set { m_x = value; }
			}

			public int Y
			{
				get { return m_y; }
				set { m_y = value; }
			}

			public string  Text
			{
				get { return m_text; }
				set { m_text = value; }
			}

			public virtual void Display()
			{
				int newY = Console.WindowTop + m_y;
				int newX = Console.WindowLeft + m_x;
				Console.SetCursorPosition(newX, newY);
				Console.WriteLine(m_text);
			}

		}
	}
}
