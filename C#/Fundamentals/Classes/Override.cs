using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Classes
{
	class BaseEmployee
	{
		protected int m_salary;
		public int Salary
		{
			get { return m_salary; }
			set { m_salary = value; }
		}

		protected string m_name;
		public string  Name
		{
			get { return m_name; }
			set { m_name = value; }
		}


		public BaseEmployee(
			string	name,
			int		salary
			)
		{
			this.m_name = name;
			this.m_salary = salary;
		}

		public virtual int calculateSalary(
			)
		{
			return m_salary;
		}
	}

	class SpecificEmployee 
		: BaseEmployee
	{
		private int m_bonus;

		public int Bonus
		{
			get { return m_bonus; }
			set { m_bonus = value; }
		}

		public SpecificEmployee(
			string name,
			int salary,
			int bonus) : base(name, salary)
		{
			this.m_bonus = bonus;
		}

		public override int calculateSalary(
			)
		{
			return Salary + m_bonus;
		}
	}


	class Override
	{
		public static void run()
		{
			BaseEmployee alice = new BaseEmployee("Alice", 20000);
			SpecificEmployee bob = new SpecificEmployee("Bob", 20000, 10000);

			Console.WriteLine(
				String.Format(
					"Bob's salary: {0:C} and Alice's salary: {1:C}",
					alice.calculateSalary(),
					bob.calculateSalary()));

			return;

		}
	}
}
