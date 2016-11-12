using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0712
	{
		class Test
		{
			public static void process()
			{
				Random randomizer = new Random();
				Point3D[] points = new Point3D[5];
				for(int i = 0; i < 5; ++i)
				{
					points[i] = new Point3D(randomizer.Next(-10, 10), randomizer.Next(-10, 10), randomizer.Next(-10, 10));
					Console.WriteLine(points[i].ToString());
				}

				Point3D basicPoint = points[0];
				for (int i = 1; i < 5; ++i)
				{
					Console.WriteLine("Distance: {0}", basicPoint.DistanceTo(points[i]));
				}
			}
		}
	}
}
