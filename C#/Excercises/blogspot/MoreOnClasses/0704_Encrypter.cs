using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0704
	{
		class Encrypter
		{
			public static string Encrypt(
				string payload
				)
			{
				char[] chars = payload.ToCharArray();
				for(int i = 0; i < chars.Length; ++i)
				{
					chars[i] = (char)((int)chars[i] + 1);
				}

				return new string(chars);
			}

			public static string Decrypt(
				string payload
				)
			{
				char[] chars = payload.ToCharArray();
				for(int i = 0; i < chars.Length; ++i)
				{
					chars[i] = (char)((int)chars[i] - 1);
				}

				return new string(chars);
			}
		}
	}
}
