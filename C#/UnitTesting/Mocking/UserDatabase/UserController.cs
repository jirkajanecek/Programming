using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UserDatabase
{
	public class UserController
	{
		private IDatabase m_db;
		private long m_usersInDB;

		public IDatabase Database
		{
			get { return m_db; }
			set { m_db = value; }
		}

		public UserController(
			IDatabase db)
		{
			m_db = db;
			m_usersInDB = m_db.Count();
		}


		public long AddUser(
			string name)
		{
			long id = m_db.AddUser(name);
			m_usersInDB = m_db.Count();
			return id;
		}

		public void RemoveUser(
			long id)
		{
			if(!m_db.CheckUser(id))
			{
				throw new Exception("User not exists");
			}

			m_db.RemoveUser(id);
		}
	}
}
