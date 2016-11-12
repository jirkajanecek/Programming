using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Basic
{
	class _09
	{
		public static void process(
			bool automatic = true)
		{
			string[] labels = { "first", "second", "third", "fourth"};
			int[] automaticValues = {10,15,20,30};
			int cumulative = 0, temp = 0;

			StringBuilder result = new StringBuilder("The average of: ");

			for(int i = 0; i < 4; ++i)
			{
				if (automatic)
				{
					Console.WriteLine("Enter the {0} number: {1}", labels[i], automaticValues[i]);
					temp = automaticValues[i];
				}
				else
				{
					Console.WriteLine("Enter the {0} number: ", labels[i]);
					temp = Convert.ToInt32(Console.ReadLine());
				}

				cumulative += temp;
				result.AppendFormat("{0} ", temp);
			}

			result.AppendFormat("is: {0}", cumulative / 4);
			Console.WriteLine(result);
		}
	}
}
