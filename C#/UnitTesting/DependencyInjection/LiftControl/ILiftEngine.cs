using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LiftControl
{
	/// <summary>
	/// Definitioin for basic lift engine operations
	/// </summary>
	public interface ILiftEngine
	{
		/// <summary>
		/// method for engine activation
		/// </summary>
		/// <param name="direction">Direction in which engine should run</param>
		void ActivateEngine(
			Direction direction,
			int diffLevel);

		void DeactivateEngine();
	}
}
