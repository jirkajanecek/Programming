using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WhatsNewInCSharp6.DictionaryInitializer
{
	class BasicIdea
	{
		Dictionary<string, string> Dictionary { get; } = new Dictionary<string, string>()
		{
			["key0"] = "test0",
			["key1"] = "test1",
			["key2"] = "repeatedTestValue",
			["key3"] = "repeatedTestValue"
		};
	}
}
