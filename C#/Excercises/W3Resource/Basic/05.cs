using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Basic
{
	class _05
	{
		public static void process(
			bool automatic = true
			)
		{
			/*
			 Write a C# Sharp program to swap two numbers. Go to the editor
			Test Data:
			 Input the First Number : 5
			 Input the Second Number : 6
			 Expected Output: 
			 After Swapping :
			 First Number : 6 
			 Second Number : 5 
			 */

			int firstNumber = 0;
			int secondNumber = 0;
			int temp = 0;

			Console.Write("Input the First Number : ");
			if (!automatic)
			{
				firstNumber = Convert.ToInt32(Console.ReadLine());
			}
			else
			{
				firstNumber = 5;
				Console.WriteLine("{0}",firstNumber);				
			}

			Console.Write("Input the Second Number : ");
			if (!automatic)
			{
				secondNumber = Convert.ToInt32(Console.ReadLine());
			}
			else
			{
				secondNumber = 6;
				Console.WriteLine("{0}", secondNumber);
			}

			temp = firstNumber;
			firstNumber = secondNumber;
			secondNumber = temp;

			Console.WriteLine("After swapping : ");
			Console.WriteLine("First Number : {0}", firstNumber);
			Console.WriteLine("Second Number : {0}", secondNumber);
		}
	}
}
