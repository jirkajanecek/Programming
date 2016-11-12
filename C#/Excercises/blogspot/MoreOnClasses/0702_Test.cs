using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0702
	{
		class Test
		{
			public static void process()
			{
				Door door = new Door();
				door.Color = "Blue";

				SmallApartment smallAppartment = new SmallApartment();
				smallAppartment.Door = door;

				Person owner = new Person();
				owner.Name = "Joe Doe";
				owner.House = smallAppartment;

				owner.ShowData();
			}
		}
	}
}
