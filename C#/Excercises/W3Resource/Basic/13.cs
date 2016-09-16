using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Basic
{
	class _13
	{
		static public void process(
			bool automatic = true)
		{
			int number = 5;
			if (automatic)
			{
				Console.WriteLine("Enter a digit: 5");
			}
			else
			{
				Console.WriteLine("Enter a digit: ");
				number = Convert.ToInt32(Console.ReadLine());
			}

			int rowLimit = 5;

			for (int row = 0; row < rowLimit; ++row)
			{
				for (int collumn = 0; collumn < 3; ++collumn)
				{
					if(
						(collumn == 1) &&
						(row != 0) &&
						(row != (rowLimit - 1)))
					{
						Console.Write(" ");
					}
					else
					{
						Console.Write(number);
					}
				}

				Console.WriteLine();
			}
		}
	}
}
