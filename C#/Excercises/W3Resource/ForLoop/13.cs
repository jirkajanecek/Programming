using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace ForLoop
{
	class _13
	{
		public static void process()
		{
			int currentItem = 1;
			int itemsInLevel = 1;
			int itemsCount = 10;
			Debug.Assert(itemsCount < 100);
			int levelsCount = getLevelCount(itemsCount);
			int initSpacesInLevel = levelsCount - 1;

			for (int row = 0; row < levelsCount; ++row, --initSpacesInLevel, ++itemsInLevel)
			{
				string spaces;
				getSpaces(out spaces, initSpacesInLevel);
				Console.Write(spaces);

				for(int i = 0; i < itemsInLevel; ++i, ++currentItem)
				{
					if(currentItem > itemsCount)
					{
						break;
					}

					if (currentItem < 10)
					{
						Console.Write("{0} ", currentItem);
					}
					else
					{
						Console.Write(currentItem);
					}

				}

				Console.WriteLine();

				if (currentItem > itemsCount)
				{
					break;
				}				
			}
		}


		public static int getLevelCount(
			int itemsCount
			)
		{
			int levelsCount = 0;
			if (itemsCount == 0)
			{
				return levelsCount;
			}

			int itemsInLevel = 1;
			int remainingItems = itemsCount;

			while (remainingItems > 0)
			{
				remainingItems -= itemsInLevel;
				levelsCount++;
				itemsInLevel++;
			}

			return levelsCount;
		}


		static void getSpaces(
			out string spaces, 
			int count
			)
		{
			spaces = new string(' ', count);
			return;
		}
	}
}
