using System.Collections.Generic;

namespace WhatsNewInCSharp6.AutoPropertyInit
{
	public class BasicIdea
	{
		//read only
		public int ReadOnlyNumber { get; } = 23;
		public string ReadOnlyString { get; } = "ro string";
		public Dictionary<string, string> ReadOnlyDictionary { get; } = new Dictionary<string, string>()
		{
			["test0"] = "test",
			["test1"] = "test"
		};

		//read/write
		public int ReadWriteNumber { get; set; } = 24;
		public string ReadWriteString { get; set; } = string.Empty;
	}
}
