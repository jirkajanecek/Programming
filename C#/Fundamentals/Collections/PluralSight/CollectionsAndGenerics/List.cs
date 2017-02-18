using System.Collections.Generic;

namespace Collections.PluralSight.CollectionsAndGenerics
{
	public class List<T> : Operations<T>
	{
		private readonly System.Collections.Generic.List<T> m_list = null;

		public List(IEnumerable<T> data)
		{
			m_list = new System.Collections.Generic.List<T>(data);
		}

		public override T[] AddElement(T value, string key = null)
		{
			m_list.Add(value);
			return GetData(m_list);
		}
	}
}
