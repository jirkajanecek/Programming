using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0704
	{
		class Test
		{
			public static void process()
			{
				string payload = "Hello";
				string encrypted = Encrypter.Encrypt(payload);
				string decrypted = Encrypter.Decrypt(encrypted);
				if(decrypted.CompareTo(payload) != 0)
				{
					Debug.Assert(false);
				}
			}
		}
	}
}
