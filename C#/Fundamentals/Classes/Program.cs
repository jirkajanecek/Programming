using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Classes
{
	class Program
	{
		static void Main(string[] args)
		{
			TestClass testClass = new TestClass();
			testClass.Member03 = 3;
			testClass.member01 = 1;

			int sum = testClass.sumMembers();
			Console.WriteLine(sum);

			Console.ReadLine();
		}
	}

	class TestClass
	{
		//created by prop
		public int member01 { get; set; }
		
		//created by porpfull
		private int m_member03;
		public int Member03
		{
			get
			{
				return m_member03;
			}

			set
			{
				m_member03 = value;
			}
		}

		public int sumMembers()
		{
			return member01 + Member03;
		}

	}
}
