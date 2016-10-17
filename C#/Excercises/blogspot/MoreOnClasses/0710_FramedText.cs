using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0710
	{
		class FramedText : CenteredText
		{
			char[] m_asterixis = null;

			public FramedText(
				int y,
				string text
				) : base(y, text)
			{
				m_asterixis = new char[m_text.Length + 2];
				for (int i = 0; i < m_asterixis.Length; ++i)
				{
					m_asterixis[i] = '*';
				}
			}

			public override void Display()
			{
				int startPointX = GetStartPoint() - 1;
				Console.SetCursorPosition(startPointX, Console.WindowTop + m_y);
				Console.WriteLine(m_asterixis);
				Console.SetCursorPosition(startPointX, Console.WindowTop + m_y + 1);
				Console.WriteLine("*{0}*", m_text);
				Console.SetCursorPosition(startPointX, Console.WindowTop + m_y + 2);
				Console.WriteLine(m_asterixis);
			}
		}
	}
}
