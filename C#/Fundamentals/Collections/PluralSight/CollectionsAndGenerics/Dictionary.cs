using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Collections.PluralSight.CollectionsAndGenerics
{
	public class Dictionary<T> : Operations<T>
	{
		private readonly System.Collections.Generic.Dictionary<string, T> m_dictionary = null;

		public Dictionary(string[] keys, T[] data)
		{
			if (keys.Count() != data.Count())
			{
				throw new ArgumentException();
			}

			m_dictionary = new System.Collections.Generic.Dictionary<string, T>();
			for (int i = 0; i < keys.Count(); ++i)
			{
				m_dictionary.Add(keys[i], data[i]);
			}
		}

		public override T[] AddElement(T value, string key = null)
		{
			m_dictionary.Add(key, value);
			return GetData(m_dictionary);
		}
	}
}
