using System;
using System.Timers;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EvenDrivenProgramming
{
	class BobTabor
	{
		public static void BobTaborCallback()
		{
			Timer timer = new Timer(2000);

			timer.Elapsed += Timer_Elapsed;
			timer.Elapsed += Timer_Elapsed1;
			timer.Start();

			Console.WriteLine("Press enter to remove yellow timer.");
			Console.ReadLine();
			timer.Elapsed -= Timer_Elapsed1;
		}

		private static void Timer_Elapsed1(object sender, ElapsedEventArgs e)
		{
			Console.ForegroundColor = ConsoleColor.Yellow;
			Console.WriteLine("Elapsed: {0:HH:mm:ss.fff}", e.SignalTime);
		}

		private static void Timer_Elapsed(object sender, ElapsedEventArgs e)
		{
			Console.ForegroundColor = ConsoleColor.Blue;
			Console.WriteLine("Elapsed: {0:HH:mm:ss.fff}", e.SignalTime);
		}
	}
}
