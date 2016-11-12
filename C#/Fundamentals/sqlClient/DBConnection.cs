using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SqlClient;
using System.Data.SqlTypes;
using System.Data;
using System.Diagnostics;

namespace sqlClient
{
	class DBTester
	{
		public void Connect(
			string host,
			string name,
			string psw,
			string db,
			ref SqlConnection sqlConnection
		)
		{
			sqlConnection = new SqlConnection();
			sqlConnection.ConnectionString = "Server=" + host + ";Port=3306;Database=" + db + ";Uid=" + name + ";Pwd=" + psw + ";";
			try
			{
				sqlConnection.Open();
			}
			catch(InvalidOperationException e)
			{
				Console.WriteLine("Something is fucked: " + e.Message);
				sqlConnection = null;
			}
			catch(System.Exception e)
			{
				Console.WriteLine("Generally it is fucked: " + e.Message);
				sqlConnection = null;
			}
		}

		public void ProcessSelect(
			ref SqlConnection sqlConnection
			)
		{
			SqlCommand command = new SqlCommand("SELECT * FROM pokusna WHERE jedna=@paramName", sqlConnection);

			command.Parameters.Add(new SqlParameter("paramName", 1));
			command.Parameters.Add("paramName", SqlDbType.Int).Value = 1;

			try
			{
				using (SqlDataReader reader = command.ExecuteReader())
				{
					while (reader.Read())
					{
						int converted = reader.GetInt32(0); //get it converted
						Console.Write(converted);
						Console.WriteLine(reader[0]);
					}
					//reader.Close(); managed by using
				}
			}
			catch(SqlException e)
			{
				Console.WriteLine("Something is fucked: " + e.Message);
			}
		}

		public void ProcessInsert(
			ref SqlConnection sqlConnection)
		{
			Random randomizer = new Random();
			SqlCommand command = new SqlCommand("INSERT INTO pokusna (jedna) VALUES(@0)", sqlConnection);
			command.Parameters.Add("0", SqlDbType.Int).Value = randomizer.Next(1, 10);

			try
			{
				int affectedRows = command.ExecuteNonQuery();
				Debug.Assert(affectedRows == 1);
			}
			catch(SqlException e)
			{
				Console.WriteLine("Something is fucked again:" + e.Message);
			}
			
		}
	}
}
