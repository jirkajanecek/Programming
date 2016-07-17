using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Collections
{
    class Program
    {
        static void Main(string[] args)
        {
            MVA();

			Console.ReadLine();

        }

        static void MVA(
            )
        {
			Car car1 = new Car();
			car1.Model = "Octavia";
			car1.Make = "Skoda";
			car1.VIN = "123";

			Car car2 = new Car();
			car2.Model = "Fabia";
			car2.Make = "Skoda";
			car2.VIN = "234";


			Book book1 = new Book();
			book1.Author = "Jiri Janecek";
			book1.Title = "Captain's log";
			book1.ISBN = "0-NCC-17016-E";

			//old version of collection
			//ArrayList
			ArrayList arrayList = new ArrayList();
			arrayList.Add(car1);
			arrayList.Add(car2);
			arrayList.Add(book1);   //! wow - not strongly typed

			try
			{
				foreach (Car car in arrayList)
				{
					Console.WriteLine(car.Model);
				}
			}
			catch (InvalidCastException)
			{
				Console.WriteLine("Book in car list...");
			}

			// List<T> - generic collection

			List<Car> cars = new List<Car>();
			cars.Add(car1);
			cars.Add(car2);

			foreach(Car car in cars)
			{
				Console.WriteLine(car.Model);
			}

			// Dictionary<TKey, TValue>

			Dictionary<string, Car> dictionary = new Dictionary<string, Car>();
			dictionary.Add(car1.VIN, car1);
			dictionary.Add(car2.VIN, car2);

			Console.WriteLine(dictionary["123"].Model);

			string[] names = { "Bob", "John", "George", "Alice" };

			//object initializer syntax
			Car car3 = new Car() { Make = "Skoda", Model = "Rapid", VIN = "456" };
			Car car4 = new Car() { Make = "Skoda", Model = "Yeti", VIN = "567" };

			List<Car> cars2 = new List<Car>() {
				car1,
				car2,
				car3,
				car4,
				new Car() { Make = "Skoda", Model = "Roomster", VIN = "678" }
			};

			foreach(Car car in cars2)
			{
				Console.WriteLine(car.Model);
			}

			return;
        }
    }

    class Car
    {
		public string VIN { get; set; }
		public string Model { get; set; }
        public string Make { get; set; }
    }

    class Book
    {
        public string Title { get; set; }
        public string Author { get; set; }
        public string ISBN { get; set; }

    }
}
