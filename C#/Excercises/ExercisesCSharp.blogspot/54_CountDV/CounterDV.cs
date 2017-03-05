using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _54_CountDV
{
	public class CounterDV
	{
		static public void CountDV(string text, ref int digits, ref int vovels)
		{
			var arrayedText = text.ToLower().ToCharArray();

			digits = arrayedText.Where(v => char.IsDigit(v)).Count();
			vovels = arrayedText.Where(v => IsVovel(v)).Count();

			return;
		}

		static private bool IsVovel(char b)
		{
			var vovels = new char[]{ 'a', 'e', 'i', 'o', 'u', 'y'};
			return vovels.Contains(char.ToLower(b));
		}
	}
}
