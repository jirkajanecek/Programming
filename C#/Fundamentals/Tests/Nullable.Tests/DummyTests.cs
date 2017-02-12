using NUnit.Framework;


namespace Nullable.Tests
{
	[TestFixture]
	class DummyTests
	{
		[Test]
		public void GetNumbericValue_Default()
		{
			var dummy = new Dummy();

			int expected = 0;
			dummy.Numeric = null;
			var actual = dummy.GetNumericValueViaApi();
			Assert.AreEqual(expected, actual, "default numeric value differs");

			expected = 10;
			dummy.Numeric = expected;
			actual = dummy.GetNumericValueViaApi();
			Assert.AreEqual(expected, actual, "set up numeric value differs");
		}

		[Test]
		public void GetNumbericValue_IfStatement()
		{
			var dummy = new Dummy();

			int expected = 0;
			dummy.Numeric = null;
			var actual = dummy.GetNumericValueViaIfStatement();
			Assert.AreEqual(expected, actual, "default numeric value differs");

			expected = 10;
			dummy.Numeric = expected;
			actual = dummy.GetNumericValueViaIfStatement();
			Assert.AreEqual(expected, actual, "set up numeric value differs");
		}

		[Test]
		public void GetNumericValue_CoalescingOperator()
		{
			var dummy = new Dummy();

			int expected = 5;			
			dummy.Numeric = null;
			var actual = dummy.GetNumericValueViaCoalescingOperator();
			Assert.AreEqual(expected, actual, "null int? default value differs");

			expected = 10;
			dummy.Numeric = expected;
			actual = dummy.GetNumericValueViaCoalescingOperator();
			Assert.AreEqual(expected, actual, "set up int? default value differs");
		}

		[Test]
		public void CallingMethod_NullConditionalOperator()
		{
			var dummy = new Dummy();

			var expected = "";			
			dummy.Numeric = null;
			var actual = dummy.ConvertToStringViaNullConditionalOperator();
			Assert.AreEqual(expected, actual, "null int? converted value differs");

			expected = "10";
			dummy.Numeric = 10;
			actual = dummy.ConvertToStringViaNullConditionalOperator();
			Assert.AreEqual(expected, actual, "set up int? converted value differs");
		}

		[Test]
		public void GetValueFromIndex_NullConditionalOperator()
		{
			var dummy = new Dummy();

			var expected = 0;
			dummy.NumericArray = null;
			var actual = dummy.GetValueFromIndexViaNullConditionalOperator(1);
			Assert.AreEqual(expected, actual, "null int array numeric value differs");

			expected = 9;
			dummy.NumericArray = new int[]{ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
			actual = dummy.GetValueFromIndexViaNullConditionalOperator(1);
			Assert.AreEqual(expected, actual, "set up int array numeric value differs");
		}
	}
}
