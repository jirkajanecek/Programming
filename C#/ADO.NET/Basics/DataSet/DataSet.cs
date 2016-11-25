using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data;
using System.Data.SqlClient;

namespace DataSet
{
	class DataSet
	{
		string m_connectionString = @"Data Source=(localdb)\ProjectsV13;Initial Catalog=CarEvidence;Integrated Security=True;Connect Timeout=30;Encrypt=False;TrustServerCertificate=True;ApplicationIntent=ReadWrite;MultiSubnetFailover=False";
		SqlConnection m_connection = null;		

		static public void Run()
		{
			DataSet set = null;
			try
			{
				set = new DataSet();
				set.Init();
				set.Process();
			}
			catch(Exception ex)
			{
				Console.WriteLine("Exception {0}: {1}",
					ex.GetType().ToString(), ex.Message);
			}
			finally
			{
				if(set != null)
				{
					set.Finish();
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
			//update
		}

		public void Finish()
		{
			if(m_connection != null)
			{
				m_connection.Dispose();
			}
		}

		public void Selects()
		{
			SimpleSelect();
			RelationedSelect(); ;
		}

		private void SimpleSelect()
		{
			string query = "SELECT * FROM c_color;";

			using (SqlCommand command = new SqlCommand(query, m_connection))
			using (SqlDataAdapter dataAdapter = new SqlDataAdapter(command))
			{
				System.Data.DataSet dataSet = new System.Data.DataSet("Colors");
				dataAdapter.Fill(dataSet, "c_color");

				foreach (DataRow row in dataSet.Tables["c_color"].Rows)
				{
					Console.WriteLine("{0} - {1}",
						row["Id"], row["Color"]);
				}
			}
		}

		private void RelationedSelect()
		{
			string qCars = "SELECT * FROM cars;";
			string qColors = "SELECT * FROM c_color;";

			using (var ds = new System.Data.DataSet("carsColors"))
			using (var daCars = new SqlDataAdapter(qCars, m_connection))
			using (var daColors = new SqlDataAdapter(qColors, m_connection))
			{
				daCars.Fill(ds, "cars");
				daColors.Fill(ds, "c_color");				

				DataRelation drCars2Color = ds.Relations.Add(
					"cars2Color",
					ds.Tables["c_color"].Columns["Id"],
					ds.Tables["cars"].Columns["id_color"]);

				foreach (DataRow colorsRow in ds.Tables["c_color"].Rows)
				{
					string color = colorsRow["Color"].ToString();	
					foreach (DataRow carsRow in colorsRow.GetChildRows(drCars2Color))
					{
						Console.WriteLine("{0} - {1} - {2}",
							carsRow["Vin"], carsRow["DoorCount"], color);
					}
				}				
			}
		}
	}
}
