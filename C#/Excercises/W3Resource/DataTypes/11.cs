using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataTypes
{
	class _11
	{
		public static void process(
			bool automatic = true
			)
		{
			StringBuilder binaryForm = new StringBuilder();
			char[] charsToTrim = new char[] { '0' };
			ulong input = 256;
			if(!automatic)
			{
				Console.WriteLine("Number to convert: ");
				input = Convert.ToUInt64(Console.ReadLine());
			}

			for(int i = 0, n = 63; i < 64; i++, --n)
			{
				ulong power = getPowerValue(n);
				if (input / power != 0)
				{
					binaryForm.Append("1");
				}
				else
				{
					binaryForm.Append("0");
				}
				input = input % power;
			}

			Console.WriteLine(binaryForm.ToString().TrimStart(charsToTrim));			
		}

		private static ulong getPowerValue(
			int power
			)
		{
			ulong value = 1;
			for(int i = 0; i < power; ++i)
			{
				value *= 2;
			}
			return value;
		}
	}
}
