using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using _72_Square;
using NUnit.Framework;

namespace _72_Square.Tests
{
	[TestFixture]
	class SquareTests
	{
		[TestCase(3, 6)]
		[TestCase(10, 20)]
		public void Square_Scale(int side, int expectedSide)
		{
			var square = new Square(side);
			square.Scale();

			Assert.AreEqual(expectedSide, square.Side1, "Side 1 should be equal");
			Assert.AreEqual(expectedSide, square.Side2, "Side 2 should be equal");
		}

		[TestCase(6,6)]
		[TestCase(-2,-2)]
		public void Square_Move(int newX, int newY)
		{
			var square = new Square(1);
			square.Move(newX, newY);

			var location = square.Location;
			Assert.AreEqual(newX, location.X, "X-axis should be equal");
			Assert.AreEqual(newY, location.Y, "Y-axis should be equal");
		}
	}
}
