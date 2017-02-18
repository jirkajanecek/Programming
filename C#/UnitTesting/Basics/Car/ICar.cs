using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cars
{
	interface ICar
	{
		/// <summary>
		/// Interface method for driving
		/// </summary>
		/// <param name="time">Time in minutes</param>
		/// <param name="speed">Speed in kilometers per hour</param>
		void Drive(
			double time,
			double speed
			);
	}
}
