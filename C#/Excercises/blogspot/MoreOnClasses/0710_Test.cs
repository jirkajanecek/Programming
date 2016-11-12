using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0710
	{
		class Test
		{
			public static void process()
			{
				int cursorX = Console.CursorLeft;
				int cursorY = Console.CursorTop;

				ScreenText screenText = new ScreenText(5, 5, "***AHOJ***");
				screenText.Display();

				CenteredText centeredText = new CenteredText(10, "*****NAZDAR*****");
				centeredText.Display();

				FramedText framedText = new FramedText(15, "CUS");
				framedText.Display();

				Console.SetCursorPosition(cursorX, cursorY);
			}
		}
	}
}
