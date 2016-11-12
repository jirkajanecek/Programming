using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataTypes
{
	class _02
	{
		public static void process(
			bool automatic = true
			)
		{
			int number = 6, width = 6;

			if(!automatic)
			{
				Console.WriteLine("Enter a number: ");
				number = Convert.ToInt32(Console.ReadLine());
				Console.WriteLine("Enter the desired width: ");
				width = Convert.ToInt32(Console.ReadLine());
			}

			for(int i = 0; i < width; ++i)
			{
				for(int j = 0; j < width - i; ++j)
				{
					Console.Write(number);
				}
				Console.WriteLine();
			}
		}
	}
}
