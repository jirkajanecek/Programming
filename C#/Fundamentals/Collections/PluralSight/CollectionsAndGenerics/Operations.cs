using System.Collections.Generic;
using System.Linq;

namespace Collections.PluralSight.CollectionsAndGenerics
{
	public abstract class Operations<T>
	{
		protected T[] GetData(IList<T> collection)
		{
			return collection.ToArray();
		}

		protected T[] GetData(IDictionary<string, T> collection)
		{
			return collection.Values.ToArray();
		}

		public abstract T[] AddElement(T value, string key = null);
	}
}
