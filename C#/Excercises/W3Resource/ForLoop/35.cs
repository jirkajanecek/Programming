using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ForLoop
{
	class _35
	{
		public static void process(
			)
		{
			int members = 15;
			Debug.Assert(members > 2);
			
			int firstMember = 0;
			int secondMember = 1;
			Console.Write("Here is the fibonacci series upto to {0} terms : ", members);
			for(int currentMember = 2; currentMember < members; ++currentMember)
			{
				int newMember = firstMember + secondMember;
				Console.Write("{0} ", newMember);
				firstMember = secondMember;
				secondMember = newMember;
			}
			Console.WriteLine();
		}
	}
}
