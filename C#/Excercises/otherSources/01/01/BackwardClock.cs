using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _01
{
	class BackwardClock
		: Clock
	{
		public override void ProcessTick()
		{
			m_ticks -= 1;
			if (m_ticks == -1)
			{
				m_ticks = m_ticksLimit - 1;
			}
		}

		public override void PrintTime()
		{
			Console.Write("Backward clock: ");
			base.PrintTime();
		}
	}
}
