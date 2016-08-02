using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EvenDrivenProgramming
{
	class EventClass
	{
		public static void ProcessEventClass()
		{
			return;
		}
	}

	//event arguments class
	//used for sending information through event
	public class PublisherEventArgs: EventArgs
	{
		class SingletonRandom
		{
			private static SingletonRandom m_instance;

			private Random m_random;

			public static SingletonRandom Instance
			{
				get
				{
					if(m_instance == null)
					{
						m_instance = new SingletonRandom();
					}
					return m_instance;
				}
			}

			private SingletonRandom()
			{
				m_random = new Random();
			}

			public int GetNewID()
			{
				return m_random.Next();
			}
		}

		public PublisherEventArgs(string s)
		{
			m_msg = s;
			m_id = SingletonRandom.Instance.GetNewID();
		}		
		
		//generally custom data holders

		private string m_msg;
		private int m_id;


		public string Message
		{
			get { return m_msg; }
			
		}

		public int ID
		{
			get { return m_id; }			
		}

	}

	//raising events
	class EventPublisher
	{
		//non-generic event handler - using custom data
		public event EventHandler<PublisherEventArgs> NonGenericEvent;

		//generic event handler using base class EventArgs
		public event EventHandler GenericHandler;

		public void PublishEvent()
		{
			EventWrapper(new PublisherEventArgs("Event send in " + DateTime.Now.ToString()));
		}

		public void PublishEventBlank()
		{
			EventWrapperBlank(new EventArgs());
		}

		protected virtual void EventWrapper(
			PublisherEventArgs eventArgs
			)
		{
			EventHandler<PublisherEventArgs> handler = NonGenericEvent;

			//when the event is not registered, there is no need to send it
			if(handler != null)
			{
				handler(this, eventArgs);
			}
		}

		protected virtual void EventWrapperBlank(
			EventArgs args
			)
		{
			EventHandler handler = GenericHandler;

			//when the event is not registered, there is no need to send it
			if (handler != null)
			{
				handler(this, args);
			}
		}
	}

	//receiving events
	class EventSubscriber
	{
		//TODO
	}
}
