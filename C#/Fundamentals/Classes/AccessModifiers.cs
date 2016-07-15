using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Classes
{
    class AccessModifiers
    {
        static public void run(
            )
        {
            AccessModifiers modifiers = new AccessModifiers();
            modifiers.publicMethod();
            return;
        }

        public void publicMethod(
            )
        {
            Console.WriteLine(privateMethod());
        }

        private string privateMethod(
            )
        {
            return "Payload";
        }
    }
}
