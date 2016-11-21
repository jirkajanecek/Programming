using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UserDatabase
{
	public interface IDatabase
	{
		long AddUser(
			string name);

		void RemoveUser(
			long id);

		bool CheckUser(
			string name);

		bool CheckUser(
			long id);

		int Count();

	}
}
