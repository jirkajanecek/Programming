using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0703
	{
		class Test
		{
			public static void process()
			{
				Random randomizer = new Random();
				_0701.Table[] tables = new _0701.Table[10];
				for(int i = 0; i < 10; ++i)
				{
					if(i < 5)
					{
						tables[i] = new _0701.Table(randomizer.Next(50, 200), randomizer.Next(50, 200));
					}
					else
					{
						tables[i] = new CoffeeTable(randomizer.Next(40, 120), randomizer.Next(40, 120));
					}
				}

				foreach(_0701.Table table in tables)
				{
					table.ShowData();
				}
			}
		}
	}
}
