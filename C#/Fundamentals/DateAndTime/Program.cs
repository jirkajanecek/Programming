using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DateAndTime
{
	class Program
	{
		static void Main(string[] args)
		{
			tips();

			Console.ReadLine();
		}

		static void tips()
		{
			DateTime now = DateTime.Now;

			Console.WriteLine(now);
			Console.WriteLine(now.ToShortDateString());
			Console.WriteLine(now.ToShortTimeString());
			Console.WriteLine(now.ToLongDateString());
			Console.WriteLine(now.ToLongTimeString());

			DateTime addedTime = now.AddDays(1);
			Console.WriteLine(addedTime.ToString());

			DateTime substractedTime = addedTime.AddDays(-2);
			Console.WriteLine(substractedTime.ToString());

			DateTime specificTime = new DateTime(1986, 09, 08);
			Console.WriteLine(specificTime);

			DateTime specificTime01 = DateTime.Parse("09/08/1986");
			Console.WriteLine(specificTime01);

			TimeSpan lifeSpan = DateTime.Now.Subtract(specificTime);
			int days = lifeSpan.Days;
			double totalDays = lifeSpan.TotalDays;
			Console.WriteLine(String.Format("Days {0} -- Total Days {1}",days, totalDays));

		}
	}
}
