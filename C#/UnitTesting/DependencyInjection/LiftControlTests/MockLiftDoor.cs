using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using LiftControl;

namespace LiftControlTests
{
	class MockLiftDoor : ILiftDoor
	{
		public void Close()
		{
			return;
		}

		public void Open()
		{
			return;
		}
	}
}
