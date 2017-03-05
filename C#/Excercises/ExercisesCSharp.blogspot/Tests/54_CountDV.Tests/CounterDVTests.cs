using NUnit.Framework;
using _54_CountDV;

namespace _54_CountDV.Tests
{
	[TestFixture]
	class CounterDVTests
	{
		[TestCase("AHoj1234", 4, 2)]
		[TestCase("Some1Other2Text3", 3, 5)]
		public void _54_CountDV_Suceeded(string text, int expectedDigits, int expectedVovels)
		{
			int actualDigits = 0;
			int actualVovels = 0;

			CounterDV.CountDV(text, ref actualDigits, ref actualVovels);

			Assert.AreEqual(expectedDigits, actualDigits, "digits should be the same");
			Assert.AreEqual(expectedVovels, actualVovels, "vovels should be the same");
		}
	}
}
