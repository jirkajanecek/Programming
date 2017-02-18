using System;

namespace Events.ClrBook
{
	public class CustomEventReceiver
	{
		public CustomEventReceiver(CustomEventEmitter emitter)
		{
			emitter.NewCustomEvent += CustomEventHandler;			
		}

		private void CustomEventHandler(Object sender, CustomEventArgs e)
		{
			Console.WriteLine($"Message: {e.IntegerArgument}, {e.StringArgument}");
		}

		public void UnregisterCustomEvent(CustomEventEmitter emitter)
		{
			emitter.NewCustomEvent -= CustomEventHandler;
		}
	}
}
