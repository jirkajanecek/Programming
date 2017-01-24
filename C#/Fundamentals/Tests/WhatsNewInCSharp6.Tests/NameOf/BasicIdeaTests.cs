using WhatsNewInCSharp6.NameOf;
using NUnit.Framework;

namespace WhatsNewInCSharp6.Tests.NameOf
{
	[TestFixture]
	class BasicIdeaTests
	{
		[Test]
		public void UsingNameOf()
		{
			BasicIdea bi = new BasicIdea();
			Assert.AreEqual("m_param0", bi.UsingNameOf(BasicIdea.ParamType.PARAM0), "The first member's name is different");
			Assert.AreEqual("m_param1", bi.UsingNameOf(BasicIdea.ParamType.PARAM1), "The first member's name is different");
		}
	}
}
