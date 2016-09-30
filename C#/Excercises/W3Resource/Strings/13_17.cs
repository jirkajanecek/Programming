using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Strings
{
	class _13_17
	{
		public static void process()
		{
			string testString = "Quite long test string";
			_13(testString);
			_17(testString);
		}

		private static void _13(
			string testString
			)
		{
			string substring = testString.Substring(5, 5);
			string withoutLibrary = new string(testString.ToCharArray(), 5, 5);
			Debug.Assert(withoutLibrary.CompareTo(substring) == 0);
		}

		private static void _17(
			string testString
			)
		{
			string substring = testString.Substring(5, 5);
			int position = testString.IndexOf(substring);
			int result = -1;
			for (int i = 0; i < testString.Length; ++i)
			{
				if (
					(substring[0] == testString[i]) &&
					((testString.Length - i) > substring.Length))
				{
					bool miss = false;
					for (int j = 0; j < substring.Length; ++j)
					{
						if (substring[j] != testString[j + i])
						{
							miss = true;
							break;
						}
					}
					if (miss)
					{
						continue;
					}
					else
					{
						result = i;
						break;
					}
				}
			}
			Debug.Assert(result == position);
		}
	}
}
