using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0701
	{
		class Test
		{
			public static void process()
			{
				Random randomizer = new Random();
				Table[] tables = new Table[10];
				for (int i = 0; i < 10; i++)
				{
					tables[i] = new Table(randomizer.Next(50, 200), randomizer.Next(50, 200));
					tables[i].ShowData();
				}
			}
		}
	}
}
