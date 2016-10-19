using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Arrays
{
	class Program
	{
		static void Main(string[] args)
		{
			int[] numbers = new int[5];

			for(int i = 0; i < numbers.Length; ++i)
			{
				numbers[i] += i + 7;
			}

			int[] numbers2 = new int[numbers[0]];


			///////////////////////////////////////////////////////////////////

			int[] numbers3 = new int[] { 0, 12, 3, 548, 98, 68 };

			string[] strings = new string[] { "ahoj", "nazdar", "cau" };			

			foreach (string item in strings)
			{
				Console.WriteLine(item);
			}

			///////////////////////////////////////////////////////////////////

			string longSentence = "Very long sentence...";
			char[] chars = longSentence.ToCharArray();
			Array.Reverse(chars);

			///////////////////////////////////////////////////////////////////

			int[,] multiInt = new int[5, 5];
			for(int i = 0; i < 5; ++i)
			{
				for(int j = 0; j < 5; ++j)
				{
					multiInt[i,j] = 5;
				}
				
			}			
		}
	}
}
