using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Singleton
{
	public class Singleton
	{
		private static Singleton _instance = null;
		private static Object _instanceLock = new Object();

		//object can not be instantiated directly
		protected Singleton()
		{

		}

		public static Singleton Instance()
		{
			lock(_instanceLock)
			{
				if(_instance == null)
				{
					_instance = new Singleton();
				}
				return _instance;
			}
		}
	}
}
