using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace Threads
{
	public class SimpleThreadTest
	{
		public class ThreadPayload
		{
			private int m_parentThreadId;
			private int m_random;
			private StringBuilder m_stringBuilder = null;
			private Object m_lock = null;


			public ThreadPayload(
				Object			lockObject,
				StringBuilder	stringBuilder
				)
			{
				m_lock = lockObject;
				m_stringBuilder = stringBuilder;
			}


			public int ParentThreadId
			{
				get { return m_parentThreadId; }
				set { m_parentThreadId = value; }
			}

			public int Random
			{
				get { return m_random; }
				set { m_random = value; }
			}

			public StringBuilder TextHolder
			{
				get { return m_stringBuilder; }
				set { m_stringBuilder = value; }
			}

			public Object TextHolderLock
			{
				get { return m_lock; }
			}
		}

		public delegate void ThreadHandlerDelegate(ThreadPayload payload);

		public static void Process()
		{
			Object textHolderLock = new Object();
			StringBuilder textHolder = new StringBuilder();
			Random randomizer = new Random();

			Thread[] threads = new Thread[3];
			for(int i = 0; i < 3; ++i)
			{
				ThreadPayload threadPayload = new ThreadPayload(textHolderLock, textHolder);
				threadPayload.ParentThreadId = Thread.CurrentThread.ManagedThreadId;
				threadPayload.Random = randomizer.Next(100, 300);

				Console.WriteLine("{0} launching thread woth random: {1}", Thread.CurrentThread.ManagedThreadId, threadPayload.Random);
				threads[i] = new Thread(ThreadHandler);				
				threads[i].Start(threadPayload);
			}

			Console.WriteLine("All threads launched");

			foreach(Thread thread in threads)
			{
				thread.Join();
			}

			Console.WriteLine("All threads finished");
			Console.ReadLine();
		}

		private static void ThreadHandler(
			object data
			)
		{
			ThreadPayload threadData = (ThreadPayload)data;
			Random randomizer = new Random((int)DateTime.Now.Ticks);
			
			for (int i = 0; i < 3; ++i)
			{
				int slept = randomizer.Next(500, 1000);
				Thread.Sleep(slept);
				Console.WriteLine("Thread: {0} has parent thread: {1} with randon id: {2} slept: {3}",
					Thread.CurrentThread.ManagedThreadId,
					threadData.ParentThreadId,
					threadData.Random,
					slept);
			}

			lock (threadData.TextHolderLock)
			{
				Console.WriteLine("{0}: writting to shared builder", Thread.CurrentThread.ManagedThreadId);
				threadData.TextHolder.AppendFormat("-{0}-", Thread.CurrentThread.ManagedThreadId);
				Console.WriteLine("{0}: writting finished", Thread.CurrentThread.ManagedThreadId);
			}
		}

	}
}
