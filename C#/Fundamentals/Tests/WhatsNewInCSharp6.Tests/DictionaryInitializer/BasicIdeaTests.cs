using NUnit.Framework;
using WhatsNewInCSharp6.DictionaryInitializer;

namespace WhatsNewInCSharp6.Tests.DictionaryInitializer
{
	[TestFixture]
	class BasicIdeaTests
	{
		[Test]
		public void DictionaryInitializer_BasicIdea_CheckValues()
		{
			BasicIdea bi = new BasicIdea();
			Assert.IsTrue(bi.Dictionary.ContainsKey("key0"), "key key0 is missing");
			Assert.IsTrue(bi.Dictionary.ContainsKey("key1"), "key key1 is missing");
			Assert.IsTrue(bi.Dictionary.ContainsKey("key2"), "key key2 is missing");
			Assert.IsTrue(bi.Dictionary.ContainsKey("key3"), "key key3 is missing");
			Assert.AreEqual("test0", bi.Dictionary["key0"], "value for key0 is not correct");
			Assert.AreEqual("test1", bi.Dictionary["key1"], "value for key1 is not correct");
			Assert.AreEqual("repeatedTestValue", bi.Dictionary["key2"], "value for key2 is not correct");
			Assert.AreEqual("repeatedTestValue", bi.Dictionary["key3"], "value for key3 is not correct");
		}
	}
}
