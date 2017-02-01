using NUnit.Framework;
using Collections.PluralSight.CollectionsAndGenerics;

namespace Collections.Tests.PluralSight.CollectionsAndGenerics
{
	[TestFixture]
	class ListTests
	{
		string[] m_originalArray = { "item3", "item2", "item1", "item0" };

		[Test]
		public void List_Add()
		{
			var newElement = "item4";
			string[] expected = { "item3", "item2", "item1", "item0", newElement };

			List<string> list = new List<string>(m_originalArray);
			var obtained = list.AddElement(newElement);

			CollectionAssert.AreEqual(expected, obtained, "adding new element into list failed");
		}
	}
}
