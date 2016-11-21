using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UserDatabase
{
	class Database : IDatabase
	{
		List<UserType>	m_db;
		long			m_currentId;

		public Database()
		{
			m_db = new List<UserType>();
			m_currentId = 0;
		}

		public List<UserType> DB
		{
			get { return m_db; }
			set { m_db = value; }
		}

		public long AddUser(string name)
		{
			UserType uType = new UserType {id = m_currentId, userName = name };
			m_currentId++;
			return m_currentId - 1;
		}

		public bool CheckUser(long id)
		{
			foreach(UserType ut in m_db)
			{
				if(ut.id == id)
				{
					return true;
				}
			}
			return false;
		}

		public bool CheckUser(string name)
		{
			foreach(UserType ut in m_db)
			{
				if(ut.userName.CompareTo(name) == 0)
				{
					return true;
				}
			}
			return false;
		}

		public void RemoveUser(long id)
		{
			for(int i = 0; i < m_db.Count; ++i)
			{
				if(m_db[i].id == id)
				{
					m_db.RemoveAt(i);
					break;
				}
			}
			return;
		}

		public int Count()
		{
			return m_db.Count;
		}
	}
}
