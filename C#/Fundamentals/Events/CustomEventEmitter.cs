using System;
using System.Threading;

namespace Events.ClrBook
{
	public class CustomEventEmitter
	{
		public event EventHandler<CustomEventArgs> NewCustomEvent;

		protected virtual void OnNewCustomEvent(CustomEventArgs e)
		{
			EventHandler<CustomEventArgs> temp = Volatile.Read(ref NewCustomEvent);
			if(temp != null)
			{
				temp(this, e);
			}
		}

		public void SimulateNewCustomEvent(string stringArg, int intArg)
		{
			CustomEventArgs e = new CustomEventArgs(stringArg, intArg);
			OnNewCustomEvent(e);
		}
	}
}
