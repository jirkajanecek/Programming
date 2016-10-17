using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;

namespace sqlClient
{
	class Program
	{
		static void Main(string[] args)
		{
			SqlConnection sqlConnection = null;
			DBTester test = new DBTester();
			test.Connect("virtestdb.cz.avg.com", "test_janecekj", "janecekj", "weclopusis", ref sqlConnection);
			if(sqlConnection != null)
			{
				test.ProcessInsert(ref sqlConnection);
				test.ProcessSelect(ref sqlConnection);
				sqlConnection.Close();
			}					
		}
	}
}
