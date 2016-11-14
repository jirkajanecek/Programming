using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using LiftControl;

namespace LiftControlTests
{
	[TestClass]
	public class LiftTest01
	{
		MockLiftDoor m_door;
		MockLiftEngine m_engine;

		public LiftTest01()
		{
			m_door = new MockLiftDoor();
			m_engine = new MockLiftEngine();
		}

		[TestMethod]
		[ExpectedException(typeof(ArgumentException))]
		public void Lift_createLift_throws()
		{
			var lift = new Lift(m_door, m_engine, 2, 1);
		}

		[TestMethod]
		public void Lift_createLift_success()
		{
			var lift = new Lift(m_door, m_engine, -1, 8);
		}

		[TestMethod]
		public void Lift_checkLiftPlan_success()
		{
			var lift = new Lift(m_door, m_engine, -1, 8);
			lift.ActivateLift(6);

			Queue<int> liftPlan = lift.GetLiftPlan();
			Assert.AreEqual(1, liftPlan.Count);
			Assert.AreEqual(6, liftPlan.Peek());

			lift.DeactivateLift();
			Assert.AreEqual(0, liftPlan.Count);
		}

		[TestMethod]
		[ExpectedException(typeof(ArgumentOutOfRangeException))]
		public void Lift_setupWrongLevel_throws()
		{
			var lift = new Lift(m_door, m_engine, -1, 8);
			lift.ActivateLift(-2);
			lift.ActivateLift(9);
		}
	}
}
