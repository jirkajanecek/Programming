using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Nullable
{
	public class Dummy
	{
		int? m_i;
		int[] m_ia;

		public int? Numeric
		{
			get
			{
				return m_i;
			}

			set
			{
				m_i = value;
			}
		}

		public int[] NumericArray
		{
			get
			{
				return m_ia;
			}

			set
			{
				m_ia = value;
			}
		}

		public int GetNumericValueViaApi()
		{
			return m_i.GetValueOrDefault();			
		}

		public int GetNumericValueViaIfStatement()
		{
			return (m_i.HasValue) ? m_i.Value : 0;
		}

		public int GetNumericValueViaCoalescingOperator()
		{
			return m_i ?? 5;
		}

		public string ConvertToStringViaNullConditionalOperator()
		{
			var converted = m_i?.ToString();
			return converted ?? "";
		}

		public int GetValueFromIndexViaNullConditionalOperator(int index)
		{
			var temp = m_ia?[index];			
			return temp ?? 0;
		}
	}
}
