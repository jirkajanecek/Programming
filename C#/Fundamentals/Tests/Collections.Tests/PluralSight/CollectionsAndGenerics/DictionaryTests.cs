using Collections.PluralSight.CollectionsAndGenerics;
using NUnit.Framework;

namespace Collections.Tests.PluralSight.CollectionsAndGenerics
{
	[TestFixture]
	class DictionaryTests
	{
		string[] m_originalArray = { "item3", "item2", "item1", "item0" };

		[Test]
		public void Dictionary_Add()
		{
			var newElement = "item4";
			string[] expected = { "item3", "item2", "item1", "item0", newElement };

			Dictionary<string> list = new Dictionary<string>(m_originalArray, m_originalArray);
			var obtained = list.AddElement(newElement, newElement);

			CollectionAssert.AreEqual(expected, obtained, "adding new element into list failed");
		}

	}
}
