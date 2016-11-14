using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LiftControl
{
	public class Lift : ILift
	{
		ILiftEngine m_engine;
		ILiftDoor m_door;

		int m_currentLevel;
		Queue<int> m_liftPlan;

		int m_highestLevel = 0;
		int m_lowestLevel = 0;

		// Dependency injection by setter		
		public ILiftDoor Door
		{
			get { return m_door; }
			set { m_door = value; }
		}

		// Dependency injection by setter
		public ILiftEngine Engine
		{
			get { return m_engine; }
			set { m_engine = value; }
		}

		// Dependency injection by constructor
		public Lift(
			ILiftDoor door,
			ILiftEngine engine,
			int lowestLevel,
			int highestLevel)
		{
			if(lowestLevel >= highestLevel)
			{
				throw new ArgumentException("levels mismatch");
			}

			m_engine = engine;
			m_door = door;

			m_liftPlan = new Queue<int>();
			m_currentLevel = 0;

			m_highestLevel = highestLevel;
			m_lowestLevel = lowestLevel;
		}

		public void ActivateLift(
			int intentedLevel)
		{
			if(
				(intentedLevel > m_highestLevel) ||
				(intentedLevel < m_lowestLevel))
			{
				throw new ArgumentOutOfRangeException("Requested level not exists");
			}

			if(intentedLevel == m_currentLevel)
			{
				return;
			}

			m_liftPlan.Enqueue(intentedLevel);
			Direction engineDirection = (m_liftPlan.ElementAt(0) < m_currentLevel) ? Direction.DONW : Direction.UP;

			m_door.Close();
			m_engine.ActivateEngine(engineDirection, Math.Abs(m_currentLevel - intentedLevel));
		}

		public void DeactivateLift()
		{
			m_engine.DeactivateEngine();
			m_liftPlan.Dequeue();
			m_door.Open();
		}

		public Queue<int> GetLiftPlan()
		{
			return m_liftPlan;
		}
	}
}
