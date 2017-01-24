namespace WhatsNewInCSharp6.NameOf
{
	public class BasicIdea
	{
		public enum ParamType
		{
			PARAM0,
			PARAM1
		}

		private int m_param0 = 0;
		private int m_param1 = 1;

		public string UsingNameOf(ParamType pType)
		{
			switch (pType)
			{
				case ParamType.PARAM0:
					return string.Format(nameof(m_param0));
				case ParamType.PARAM1:
					return string.Format(nameof(m_param1));
				default:
					return null;
			}
		}
	}
}
