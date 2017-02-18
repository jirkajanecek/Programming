using System.Collections.Generic;

namespace Singleton.RealWorld
{
	public class Log
	{
		private static Log _instance = null;
		private static object _instanceLock = new object();

		private List<string> _container = new List<string>();
		private object _containerLock = new object();

		public string[] LogContent
		{
			get
			{
				return _container.ToArray();
			}
		}

		protected Log()
		{

		}

		public static Log GetInstance()
		{
			lock(_instanceLock)
			{
				if(_instance == null)
				{
					_instance = new Log();
				}				
			}
			return _instance;

		}

		public void LogRecord(string line)
		{
			lock(_containerLock)
			{
				_container.Add(line);
			}			
		}
	}
}
