using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MoreOnClasses
{
	namespace _0708
	{
		class RandomNumber
		{
			static int m_m = 233280;
			static int m_a = 9301;
			static int m_c = 49297;
			static int m_seed = 1;

			public static float GetFloat(
				)
			{
				m_seed = (m_seed * m_a + m_c) % m_m;
				return Math.Abs((float)m_seed / (float)m_m);
			}

			public static int GetInt(
				int max
				)
			{
				return (int)Math.Round(max * GetFloat());
			}

			public static int GetInt(
				int min,
				int max
				)
			{
				int diff = max - min;
				return min + GetInt(diff);				
			}

		}
	}
}
