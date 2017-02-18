using NUnit.Framework;
using Singleton.RealWorld;


namespace Singleton.Tests.RealWorld
{
	[TestFixture]
	class LogTests
	{
		[Test]
		public void Worker_WritingToLog()
		{
			Worker[] workers = {
				new Worker(Log.GetInstance(), "#1"),
				new Worker(Log.GetInstance(), "#2"),
				new Worker(Log.GetInstance(), "#3"),
				new Worker(Log.GetInstance(), "#4")
			};

			foreach(Worker worker in workers)
			{
				worker.DoWork();
			}

			string[] expected = {
				"Worker #1 did some work",
				"Worker #2 did some work",
				"Worker #3 did some work",
				"Worker #4 did some work"
			};

			Log log = Log.GetInstance();
			string[] actual = log.LogContent;

			CollectionAssert.AreEqual(expected, actual, "items in log differs");
		}
	}
}
