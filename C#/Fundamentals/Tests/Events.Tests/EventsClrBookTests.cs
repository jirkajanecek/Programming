using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;
using System.IO;
using Events.ClrBook;

namespace Events.Tests
{
	[TestFixture]
	class EventsClrBookTests
	{
		[Test]
		public void EventbasicFunctionality()
		{
			const string sArg = "tested argument";
			const int iArg = 8;

			string expected = string.Format($"Message: {iArg}, {sArg}\r\n");

			StringWriter sw = new StringWriter();
			Console.SetOut(sw);

			CustomEventEmitter emitter = new CustomEventEmitter();
			CustomEventReceiver receiver = new CustomEventReceiver(emitter);
			emitter.SimulateNewCustomEvent(sArg, iArg);

			string tested = sw.ToString();
			Assert.AreEqual(expected, tested, "message string format differs");
		}
	}
}
