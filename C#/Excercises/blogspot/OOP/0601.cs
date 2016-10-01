using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OOP
{
	class Person
	{
		protected int m_age;

		public void setAge(
			int age
			)
		{
			m_age = age;
		}

		public void sayHello()
		{
			Console.WriteLine("Hello...");
		}
	}

	class Teacher
		: Person
	{
		private string m_subject;

		public void Explain()
		{
			Console.WriteLine("Explanation begins...");
			return;
		}
	}

	class Student : Person
	{
		public void GoToClass()
		{
			Console.WriteLine("Going to class...");
			return;
		}

		public void ShowAge()
		{
			Console.WriteLine("I am {0} years old", m_age);
		}
	}

	class _0601
	{
		public static void process()
		{
			Person person = new Person();
			person.sayHello();

			Student student = new Student();
			student.setAge(21);
			student.sayHello();
			student.ShowAge();

			Teacher teacher = new Teacher();
			teacher.setAge(30);
			teacher.Explain();
		}
	}
}
