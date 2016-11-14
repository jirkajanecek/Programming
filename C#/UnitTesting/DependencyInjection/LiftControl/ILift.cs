using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LiftControl
{
	/// <summary>
	/// Basic operation for lift
	/// </summary>
	interface ILift
	{
		void ActivateLift(
			int intendedLevel
			);

		void DeactivateLift(
			);

		Queue<int> GetLiftPlan(
			);
	}
}
