using NUnit.Framework;
using _70_Shapes;

namespace _70_Shapes.Tests
{
	[TestFixture]
	class LocationTests
	{
		[TestCase(0,0)]
		[TestCase(10,5)]
		[TestCase(-1, -5)]
		public void Location_Init(int x, int y)
		{
			var location = new Location(x, y);

			Assert.AreEqual(x, location.X, "X-axis should be equal");
			Assert.AreEqual(y, location.Y, "Y-axis should be equal");
		}
	}
}
