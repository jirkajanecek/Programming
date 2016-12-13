using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Collections
{
    class ListTest
    {
        List<int> m_list = new List<int>();
        int[] m_payload = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        string[] m_spayload = { "a", "aa", "aaa", "aaaa", "aaaaa", "aaaaaa", "aaaaaaa", "aaaaaaaa", "aaaaaaaaa" };

        public static void Process()
        {
            var listTest = new ListTest();
            listTest.Run();
        }

        private void Run()
        {
            Add();
            All();
            Any();
            AsEnumerable();
            Average();
        }

        private void Add()
        {           
            m_list.AddRange(m_payload);
        }

        private void All()
        {
            //determines if all items succeeded in predicate
            
            //all items are odd
            Debug.Assert(m_list.All(p => p % 2 == 0) == false);

            //all items are smaller then 11
            Debug.Assert(m_list.All(p => p < 11) == true);
        }

        private void Any()
        {
            //true if any succeeded
            bool result = m_list.Any(p => p == 9);
            Debug.Assert(result == true);            
        }

        private void AsEnumerable()
        {
            IEnumerable<int> allValues = m_list.AsEnumerable();
            int[] arrayValues = m_list.ToArray();

            int i = 0;
            foreach(int value in allValues)
            {
                Debug.Assert(value == arrayValues[i]);
                i++;
            }
        }

        private void Average()
        {
            double stringLenAvg = m_spayload.Average(p => p.Length);
            Debug.Assert(stringLenAvg == 5.0d);	
        }



        


    }
}
