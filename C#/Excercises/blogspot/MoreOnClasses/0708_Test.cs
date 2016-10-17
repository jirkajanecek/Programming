using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0708
	{
		class Test
		{
			public static void process()
			{
				int min = 100, max = 150;
				for (int i = 0; i < 5000; ++i)
				{
					int random = RandomNumber.GetInt(min, max);
					Debug.Assert((min <= random) && (random <= max));
				}
			}
		}
	}
}
