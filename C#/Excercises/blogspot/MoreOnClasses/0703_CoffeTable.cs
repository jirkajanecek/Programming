using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0703
	{
		class CoffeeTable
			: _0701.Table
		{

			public CoffeeTable(
				int width,
				int height
				): base(width, height)
			{
				base.m_type = "Coffee table ";
			}			
		}
	}	
}
