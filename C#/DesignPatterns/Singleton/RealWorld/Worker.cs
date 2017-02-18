namespace Singleton.RealWorld
{
	public class Worker
	{
		protected Log _log = null;
		protected string _name = null;

		public Worker(Log log, string name)
		{
			_log = log;
			_name = name;
		}

		public void DoWork()
		{
			_log.LogRecord($"Worker {_name} did some work");
		}
	}
}
