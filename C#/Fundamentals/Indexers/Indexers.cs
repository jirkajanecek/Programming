using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Indexers
{
	class Indexers
	{
		private static int m_size = 10;
		private string[] m_payload = new string[m_size];

		Indexers()
		{
			for(int i = 0; i < m_size; ++i)
			{
				m_payload[i] = "N/A";
			}
		}

		public string this[int index]
		{
			get
			{
				if(index >= 0 && index < m_size)
				{
					return m_payload[index];
				}
				else
				{
					return null;
				}
			}

			set
			{
				if(index >= 0 && index < m_size)
				{
					m_payload[index] = value;
				}
			}
		}

		//overloaded indexer
		public int this[string payload]
		{
			get
			{
				for(int index = 0; index < m_size; ++index)
				{
					if(m_payload[index] == payload)
					{
						return index;
					}
				}

				return -1;
			}
		}

		public void printContent()
		{
			for(int i = 0; i < m_size; ++i)
			{
				Console.WriteLine("{0}", m_payload[i]);
			}
		}

		public static void process()
		{
			Indexers idxs = new Indexers();
			idxs[0] = "A";
			idxs[1] = "B";
			idxs[2] = "C";
			idxs[3] = "D";
			idxs[4] = "E";
			idxs[5] = "F";
			idxs[6] = "G";

			idxs.printContent();
			Debug.Assert(idxs["G"] == 6);
		}
	}
}
