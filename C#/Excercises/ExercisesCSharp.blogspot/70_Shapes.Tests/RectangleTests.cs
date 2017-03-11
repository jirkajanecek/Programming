using NUnit.Framework;
using _70_Shapes;
using System;

namespace _70_Shapes.Tests
{
	[TestFixture]
	class RectangleTests
	{
		[TestCase(2, 2, 2.5, 2, 5.0, 9.0)]
		[TestCase(-1, -5, 10, 3.5, 35.0, 27.0)]
		public void Rectangle_InitFull(int x, int y, double side1, double side2, double expectedArea, double expectedPerimeter)
		{
			var rect = new Rectangle(x, y, side1, side2);

			var location = rect.Location;
			Assert.AreEqual(x, location.X, "X-axis should be equal");
			Assert.AreEqual(y, location.Y, "Y-axis should be equal");

			Assert.AreEqual(side1, rect.Side1, "Side1 should be equal");
			Assert.AreEqual(side2, rect.Side2, "Side2 should be equal");

			Assert.AreEqual(expectedArea, rect.Area(), "Area should be equal");
			Assert.AreEqual(expectedPerimeter, rect.Perimeter(), "Perimeter should be equal");
		}

		[TestCase(2.5, 2, 5.0, 9.0)]
		[TestCase(10, 3.5, 35.0, 27.0)]
		public void Rectangle_InitLight(double side1, double side2, double expectedArea, double expectedPerimeter)
		{
			var rect = new Rectangle(side1, side2);

			var location = rect.Location;
			Assert.AreEqual(0, location.X, "X-axis should be equal");
			Assert.AreEqual(0, location.Y, "Y-axis should be equal");

			Assert.AreEqual(side1, rect.Side1, "Side1 should be equal");
			Assert.AreEqual(side2, rect.Side2, "Side2 should be equal");

			Assert.AreEqual(expectedArea, rect.Area(), "Area should be equal");
			Assert.AreEqual(expectedPerimeter, rect.Perimeter(), "Perimeter should be equal");
		}

		[TestCase(-1, 2)]
		[TestCase(2, -1)]
		[TestCase(-1, -2)]
		public void Rectangle_Init_ThrowsException(double side1, double side2)
		{
			Assert.Throws<ArgumentException>(delegate { new Rectangle(side1, side2); });
		}
	}
}
