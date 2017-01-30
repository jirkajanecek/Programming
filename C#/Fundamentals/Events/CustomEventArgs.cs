using System;

namespace Events.ClrBook
{
	public class CustomEventArgs : EventArgs
	{
		private readonly string m_stringArg;
		private int m_intArgs;

		public string StringArgument { get { return m_stringArg; } }
		public int IntegerArgument{	get	{ return m_intArgs;	} }

		public CustomEventArgs(string stringArg, int intArg)
		{
			m_intArgs = intArg;
			m_stringArg = stringArg;
		}
	}
}
