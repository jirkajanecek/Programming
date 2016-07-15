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
            TestClass.run();
            AccessModifiers.run();

			Console.ReadLine();
		}
	}

	public class TestClass
	{
        //static property
        private static int m_member00 = 5;
        public static int Member00
        {
            get
            {
                return m_member00;
            }

            set {
                m_member00 = value;
            } 
        }
        
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

		//common public method
		public int sumMembers()
		{
			return member01 + Member03;
		}

        public static int simMembersStaticaly(
            TestClass testClass
            )
        {
            return testClass.sumMembers();
        }

        public static void run(
            )
        {
            //current state: <ref#1> -> null
            TestClass testClass = null;

            //current state: <ref#1> -> <mem>
            testClass = new TestClass();
            testClass.Member03 = 3;
            testClass.member01 = 1;

            int sum = testClass.sumMembers();
            Console.WriteLine(sum);

            //static method are call with :: like in C++
            sum = TestClass.simMembersStaticaly(testClass);
            Console.WriteLine(sum);

            //multiplying reference to memory space allocated by new
            //current state: <ref#1> -> <mem> <- <ref#2>
            //!!not copy data - only reference to memory space!!
            TestClass testClassRef = testClass;

            //removing the reference
            //current state: <mem> <- <ref#2>
            testClass = null;

            sum = testClassRef.sumMembers();
            Console.WriteLine(sum);

            Console.WriteLine(TestClass.Member00);
        }
    }
}
