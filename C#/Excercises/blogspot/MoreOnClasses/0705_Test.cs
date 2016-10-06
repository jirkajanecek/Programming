using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0705
	{
		class Test
		{
			public static void process()
			{
				ComplexNumber a = new ComplexNumber(1, 1);
				ComplexNumber b = new ComplexNumber(2, 2);

				ComplexNumber c = a + b;
				Debug.Assert(c.Real == 3 && c.Imaginary == 3);

				int magnitude = c.GetMagnitude();
				Debug.Assert(magnitude == 18);
			}
		}
	}
}
