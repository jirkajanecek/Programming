using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _01
{
	abstract class Clock
	{
		protected int m_ticksLimit = 12*60*60;	//clock limit 12 hours only
		protected int m_ticks = 0;

		public int Ticks
		{
			get { return m_ticks; }			
		}

		public int TicksLimit
		{
			get { return m_ticksLimit; }
		}

		public virtual void ProcessTick(
			)
		{
		}
		
		public virtual void PrintTime()
		{
			int hours = m_ticks / (60 * 60);
			int minutes = m_ticks % (60 * 60) / 60;
			int seconds = m_ticks % (60 * 60) % 60;
			TimeSpan timeSpan = new TimeSpan(hours, minutes, seconds);
			Console.WriteLine("{0}:{1}:{2}", timeSpan.Hours, timeSpan.Minutes, timeSpan.Seconds);
		}
	}
}
