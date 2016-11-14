using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using LiftControl;

namespace LiftControlTests
{
	class MockLiftEngine : ILiftEngine
	{
		public void ActivateEngine(Direction direction, int diffLevel)
		{
			return;
		}

		public void DeactivateEngine()
		{
			return;
		}
	}
}
