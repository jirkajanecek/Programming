using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0710
	{
		class CenteredText 
			: ScreenText
		{
			public CenteredText(
				int y,
				string text
				) : base(0, y, text)

			{
			}

			public new int X
			{
				get { return base.m_x; }				
			}

			public override void Display()
			{
				int startPoint = GetStartPoint();
				Console.SetCursorPosition(startPoint, Console.WindowTop + m_y);
				Console.WriteLine(m_text);
			}

			protected int GetStartPoint()
			{
				return (Console.WindowWidth - m_text.Length) / 2;
			}
		}
	}
}
