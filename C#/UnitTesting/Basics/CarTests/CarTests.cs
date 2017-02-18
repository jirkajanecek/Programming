using System;
using Cars;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace CarTests
{
	[TestClass]
	public class CarTests
	{
		[TestMethod]
		public void Drive_CorrectBehaviour()
		{
			double time = 30;
			double speed = 60;

			var car = new Car();
			car.Drive(time, speed);

			double driven = car.Kilometers;
			double expected = 30;
			Assert.AreEqual(expected, driven);
		}


		[TestMethod]
		[ExpectedException(typeof(ArgumentException))]
		public void Drive_IncorrectTime_ExpectedArgumentException()
		{
			double time = -30;
			double speed = 60;

			var car = new Car();
			car.Drive(time, speed);
		}


		[TestMethod]
		[ExpectedException(typeof(ArgumentException))]
		public void Drive_IncorrectSpeed_ExpectedArgumentException()
		{
			double time = 30;
			double speed = -60;

			var car = new Car();
			car.Drive(time, speed);
		}
	}
}
