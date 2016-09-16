using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Basic
{
	class _12
	{
		public static void process(
			bool automatic = true
			)
		{
			int number = 25;
			if (automatic)
			{
				Console.WriteLine("Enter a digit: 25");
			}
			else
			{
				Console.WriteLine("Enter a digit: ");
				number = Convert.ToInt32(Console.ReadLine());
			}

			for(int i = 0; i < 4; ++i)
			{
				for(int j = 0; j < 4; ++j)
				{
					Console.Write("{0}", number);
					if(i % 2 != 0)
					{
						Console.Write(" ");
					}
				}

				Console.WriteLine();
			}
		}
	}
}
