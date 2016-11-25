using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;
using System.Data.SqlClient;

namespace DataReader
{
	class DataReader
	{
		string m_connectionString = @"Data Source = (localdb)\ProjectsV13;Initial Catalog = CarEvidence; Integrated Security = True; Connect Timeout = 30; Encrypt=False;TrustServerCertificate=True;ApplicationIntent=ReadWrite;MultiSubnetFailover=False";
		SqlConnection m_connection = null;

		public static void Run()
		{
			DataReader reader = null;
			try
			{
				reader = new DataReader();
				reader.Init();
				reader.Process();				
			}
			catch (Exception ex)
			{
				Console.WriteLine("Exception '{0}' occurred: {1}", ex.GetType().ToString(), ex.Message);
			}
			finally
			{
				if (reader != null)
				{
					reader.Finish();
				}
			}
		}

		public void Init()
		{
			m_connection = new SqlConnection(m_connectionString);			
		}

		public void Process()
		{
			Selects();
			Inserts();

			CleanUp();
		}

		public void Finish()
		{
			if(m_connection != null)
			{
				m_connection.Dispose();
			}
		}

		private void Selects()
		{
			SimpleSelect();
			ComplexSelect();
		}

		private void Inserts()
		{
			SimpleInsert();			
		}

		private void CleanUp()
		{
			string query = "DELETE FROM c_color WHERE id > 6;";
			SqlCommand command = new SqlCommand(query, m_connection);
			m_connection.Open();
			int affectedRows = command.ExecuteNonQuery();
			m_connection.Close();
			Console.WriteLine("Deleting from c_color: {0}", affectedRows);
		}

		private void SimpleSelect()
		{
			string query = "SELECT id, color FROM c_color;";
			SqlCommand command = new SqlCommand(query, m_connection);
			m_connection.Open();
			SqlDataReader sqlDataReader = command.ExecuteReader();
			while (sqlDataReader.Read())
			{
				Console.WriteLine("{0} - {1}",
					sqlDataReader["id"], sqlDataReader["color"]);
			}
			m_connection.Close();
		}

		private void ComplexSelect()
		{
			string query =
				@"SELECT o.Firstname, o.Surname, groupedCount.count
				FROM owners o
				JOIN(
					SELECT o.id AS id_owner, COUNT(o.Id) AS count
					FROM cars
					JOIN r_car_owner r
					ON r.Id_car = cars.Id
					JOIN owners o
					ON r.Id_owner = o.Id
					GROUP BY(o.Id)
					) AS groupedCount
				ON groupedCount.id_owner = o.id;";
			SqlCommand command = new SqlCommand(query, m_connection);
			m_connection.Open();
			SqlDataReader reader = command.ExecuteReader();
			while(reader.Read())
			{
				Console.WriteLine("{0} - {1} - {2}",
					reader["FirstName"], reader["Surname"], reader["count"]);
			}
			m_connection.Close();
		}

		private void SimpleInsert()
		{
			string[] additionalColors = { "purple", "gray", "orange" };
			SqlCommand command = new SqlCommand("", m_connection);

			StringBuilder query = new StringBuilder();
			int colorCounter = 0;
			foreach(string additionalColor in additionalColors)
			{
				StringBuilder parameterSignature = new StringBuilder("@param");
				parameterSignature.Append(colorCounter.ToString());
				query.AppendFormat("INSERT INTO c_color (color) VALUES ({0});", parameterSignature.ToString());
				command.Parameters.Add(new SqlParameter(parameterSignature.ToString(), additionalColor));
				colorCounter++;
			}
			command.CommandText = query.ToString();		
			m_connection.Open();
			int affectedRows = command.ExecuteNonQuery();
			Console.WriteLine("Inserting new colors: {0}", affectedRows);
			m_connection.Close();
		}

	}
}
