using Microsoft.VisualStudio.TestTools.UnitTesting;
using WhatsNewInCSharp6.AutoPropertyInit;

namespace WhatsNewInCSharp6.Tests.AutoPropertyInit
{
	[TestClass]
	public class BasicIdeaTest
	{
		private const string testString = "test";

		[TestMethod]
		public void AutoProperty_BasicIdea_CheckValues()
		{
			BasicIdea basicIdea = new BasicIdea();

			Assert.AreEqual(23, basicIdea.ReadOnlyNumber, "Read only number differs");
			Assert.AreEqual("ro string", basicIdea.ReadOnlyString, "Read only string differs");
			Assert.AreEqual(24, basicIdea.ReadWriteNumber, "Read write number differs");
			Assert.AreEqual(string.Empty, basicIdea.ReadWriteString, "Read write string differs");

			Assert.IsNotNull(basicIdea.ReadOnlyDictionary, "Read only dictionary is null");
			Assert.IsTrue(basicIdea.ReadOnlyDictionary.ContainsKey("test0"), "The first key in ro dictionary is missing");
			Assert.IsTrue(basicIdea.ReadOnlyDictionary.ContainsKey("test0"), "The second key in ro dictionary is missing");
			Assert.AreEqual(testString, basicIdea.ReadOnlyDictionary["test0"], "Test string at [test0] differs");
			Assert.AreEqual(testString, basicIdea.ReadOnlyDictionary["test1"], "Test string at [test1] differs");
		}
	}
}
