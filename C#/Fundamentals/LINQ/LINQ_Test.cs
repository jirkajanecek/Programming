using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LINQ
{
	class LINQ_Test
	{
		public static void process()
		{
			Test01();
		}

		public static void Test01()
		{
			Couple[] couples = new Couple[5];
			couples[0] = new Couple("a", "1");
			couples[1] = new Couple("b", "2");
			couples[2] = new Couple("c", "3");
			couples[3] = new Couple("d", "4");
			couples[4] = new Couple("e", "5");

			Couple[] couples2 = new Couple[5];
			couples2[0] = new Couple("a", "6");
			couples2[1] = new Couple("b", "7");
			couples2[2] = new Couple("c", "8");
			couples2[3] = new Couple("d", "9");
			couples2[4] = new Couple("e", "10");

			var concats = from 
							c in couples join
							c2 in couples2 on c.First equals c2.First
						  where Convert.ToInt32(c2.Second) >= 8
						  select new { Third = c.Second, Fourth = c2.Second };

			foreach(var concat in concats)
			{
				Console.WriteLine(concat.Third + " - " + concat.Fourth);
			}
						 
		}

		class Couple
		{
			public Couple(
				string first,
				string second
				)
			{
				First = first;
				Second = second;
			}

			public string First { get; set; }
			public string Second { get; set; }
		}
	}
}
