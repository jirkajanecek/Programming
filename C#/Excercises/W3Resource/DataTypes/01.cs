using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataTypes
{
	class _01
	{
		public static void process(
			bool automatic = true
			)
		{
			int lettersCount = 3;
			char[] letters = null;

			if (automatic)
			{
				letters = new char[] { 'b', 'a', 't' };								
			}
			else
			{
				letters = new char[lettersCount];
				for(int i = 0; i < lettersCount; ++i)
				{
					Console.WriteLine("Enter letter: ");
					letters[i] = Convert.ToChar(Console.ReadLine());
				}
			}

			List<char> reversed = letters.Reverse().ToList();
			foreach(char ch in reversed)
			{
				Console.Write("{0} ", ch);
			}
			Console.WriteLine();
		}
	}
}
