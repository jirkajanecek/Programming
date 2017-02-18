using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cars
{
	public class Car : ICar
	{
		double m_kilometers;

		public double Kilometers
		{
			get { return m_kilometers; }
		}

		public Car()
		{
			m_kilometers = 0;
		}

		/// <summary>
		/// Method for driving car
		/// </summary>
		/// <param name="time">Time in minutes</param>
		/// <param name="speed">Speed in kilometers per hour</param>
		public void Drive(
			double time,
			double speed
			)
		{
			if(time <= 0)
			{
				throw new ArgumentException("Driving time is incorrect","time");
			}

			if (speed <= 0)
			{
				throw new ArgumentException("Driving speed is incorrect","speed");
			}

			double hours = time / (double)60;
			double kilometers = speed * hours;
			m_kilometers += kilometers;
		}

	}
}
