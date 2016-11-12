using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _01
{
	class ForwardClock
		: Clock
	{
		public override void ProcessTick()
		{
			m_ticks += 1;
			if(m_ticks == m_ticksLimit)
			{
				m_ticks = 0;
			}
		}

		public override void PrintTime()
		{
			Console.Write("Forward clock: ");
			base.PrintTime();
		}
	}
}
