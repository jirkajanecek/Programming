using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataTypes
{
	class _09
	{
		public static void process(
			bool automatic = true
			)
		{
			StringBuilder answer = new StringBuilder();
			string input = "a";
			char tested = '\0';

			if (!automatic)
			{
				Console.WriteLine("Input a symbol: ");
				input = Console.ReadLine();
			}

			tested = input[0];
			if ((tested >= 'A') && (tested <= 'Z'))
			{
				answer.Append("uppercase ");				
			}
			else
			{
				answer.Append("lowercase ");
			}

			tested = tested.ToString().ToLower()[0];
			if ((tested >= 'a') && (tested <= 'z'))
			{
				switch (tested)
				{
					case 'a':
					case 'e':
					case 'i':
					case 'o':
					case 'u':
					case 'y':
						answer.Append("vowel");
						break;

					default:
						answer.Append("consonant");
						break;
				}
			}
			else if((tested >= '1') && (tested <= '0'))
			{
				answer.Append("digit");
			}
			else
			{
				answer.Append("other char");
			}

			Console.WriteLine(answer.ToString());
		}
	}
}
