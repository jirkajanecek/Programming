using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UnderstandingScope
{
    class Program
    {
        private static string sm_privateField = "";

        static void Main(string[] args)
        {
            for (int i = 0; i < 10; i++)
            {
                Console.WriteLine(i);
                sm_privateField = i.ToString();
            }

            Console.WriteLine(sm_privateField);
            Program.HelperMethod();

            Console.ReadLine();
        }

        static void HelperMethod(
            )
        {
            Console.WriteLine("Value of privet field from HelperMethod(): " + sm_privateField);
            return;
        }
    }
}
