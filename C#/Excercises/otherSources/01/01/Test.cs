using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _01
{
	class Test
	{
		public static void process()
		{
			BackwardClock bClock = new BackwardClock();
			ForwardClock fClock = new ForwardClock();

			int intersectCounter = 0;
			for (int i = 0; i < 24*60*60; ++i)	//24 hours
			{
				bClock.ProcessTick();
				fClock.ProcessTick();

				if(bClock.Ticks == fClock.Ticks)
				{
					intersectCounter++;
					bClock.PrintTime();
					fClock.PrintTime();
				}				
			}
			Console.WriteLine("Number of intersections: {0}", intersectCounter);
			Debug.Assert(intersectCounter == 4);
			return;
		}
	}
}
