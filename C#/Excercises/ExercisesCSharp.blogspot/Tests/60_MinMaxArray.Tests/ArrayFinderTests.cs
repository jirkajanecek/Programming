using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;
using _60_MinMaxArray;

namespace _60_MinMaxArray.Tests
{

	[TestFixture]
	class ArrayFinderTests
	{
		[TestCase(new float[] {1.0f, 2.0f, 3.0f, 4.0f}, 1.0f, 4.0f)]
		[TestCase(new float[] { 5.0f, 3.0f, 2.0f, 1.0f }, 1.0f, 5.0f)]
		[TestCase(new float[] { 3.0f, 6.0f, 1.0f, 2.0f }, 1.0f, 6.0f)]
		public void ArrayFinder_MinMax_Success(float[] array, float expectedMin, float expectedMax)
		{
			float actualMin = 0f, actualMax = 0f;

			ArrayFinder.MinMax(array, ref actualMin, ref actualMax);

			Assert.AreEqual(expectedMin, actualMin, "min value should be the same");
			Assert.AreEqual(expectedMax, actualMax, "max value should be the same");
		}

		[TestCase(new float[] { 1.0f, 2.0f, 3.0f, 4.0f }, 1.0f, 4.0f)]
		[TestCase(new float[] { 5.0f, 3.0f, 2.0f, 1.0f }, 1.0f, 5.0f)]
		[TestCase(new float[] { 3.0f, 6.0f, 1.0f, 2.0f }, 1.0f, 6.0f)]
		public void ArrayFinder_MinMaxIterative_Success(float[] array, float expectedMin, float expectedMax)
		{
			float actualMin = 0f, actualMax = 0f;

			ArrayFinder.MinMaxIterative(array, ref actualMin, ref actualMax);

			Assert.AreEqual(expectedMin, actualMin, "min value should be the same");
			Assert.AreEqual(expectedMax, actualMax, "max value should be the same");
		}
	}
}
