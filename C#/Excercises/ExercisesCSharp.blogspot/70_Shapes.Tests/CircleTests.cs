using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;
using _70_Shapes;

namespace _70_Shapes.Tests
{
	[TestFixture]
	class CircleTests
	{
		[TestCase(2, 2, 2, 3.1415926535897931d, 6.2831853071795862d)]
		[TestCase(-1, -5, 3.5, 5.497787143782138d, 10.995574287564276d)]
		public void Circle_InitFull(int x, int y, double diameter, double expectedArea, double expectedPerimeter)
		{
			var circle = new Circle(x, y, diameter);

			var location = circle.Location;
			Assert.AreEqual(x, location.X, "X-axis should be equal");
			Assert.AreEqual(y, location.Y, "Y-axis should be equal");

			Assert.AreEqual(diameter, circle.Diameter, "Diameter should be equal");

			Assert.AreEqual(expectedArea, circle.Area(), "Area should be equal");
			Assert.AreEqual(expectedPerimeter, circle.Perimeter(), "Perimeter should be equal");
		}

		[TestCase(2, 3.1415926535897931d, 6.2831853071795862d)]
		[TestCase(3.5, 5.497787143782138d, 10.995574287564276d)]
		public void Circle_InitLight(double diameter, double expectedArea, double expectedPerimeter)
		{
			var circle = new Circle(diameter);

			var location = circle.Location;
			Assert.AreEqual(0, location.X, "X-axis should be equal");
			Assert.AreEqual(0, location.Y, "Y-axis should be equal");

			Assert.AreEqual(diameter, circle.Diameter, "Diameter should be equal");

			Assert.AreEqual(expectedArea, circle.Area(), "Area should be equal");
			Assert.AreEqual(expectedPerimeter, circle.Perimeter(), "Perimeter should be equal");
		}

		[TestCase(-1)]		
		public void Rectangle_Init_ThrowsException(double diameter)
		{
			Assert.Throws<ArgumentException>(delegate { new Circle(diameter); });
		}
	}
}