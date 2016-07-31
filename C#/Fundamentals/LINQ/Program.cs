using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LINQ
{
	class Program
	{
		static void Main(string[] args)
		{
			List<Car> cars = new List<Car>() {
				new Car() { VIN="A1", Make = "BMW", Model= "550i", StickerPrice=55000, Year=2009},
				new Car() { VIN="B2", Make="Toyota", Model="4Runner", StickerPrice=35000, Year=2010},
				new Car() { VIN="C3", Make="BMW", Model = "745li", StickerPrice=75000, Year=2008},
				new Car() { VIN="D4", Make="Ford", Model="Escape", StickerPrice=25000, Year=2008},
				new Car() { VIN="E5", Make="BMW", Model="55i", StickerPrice=57000, Year=2010}
			};

			//SQL syntax

			//var keyword - compiler select proper type
			var bmws = from car in cars
					   where 
						car.Make == "BMW" &&
						car.Year == 2010
					   select car;
			printSet(bmws);

			//method syntax

			//lambda expression
			//given instance of collection in 'p'
			var bmwsMethod = cars.Where(p => p.Make == "BMW" && p.Year == 2010);
			printSet(bmwsMethod);

			//ordered list of cars SQL
			var orderedCars = from car in cars
						  orderby car.Year descending
						  select car;
			printSet(orderedCars);

			//ordered list of cars by method

			var orderedCarsMethod = cars.OrderByDescending(p => p.Year);
			printSet(orderedCarsMethod);

			//for each lambda
			cars.ForEach(p => Console.WriteLine("{0} {1:C}", p.Model, p.StickerPrice));
			cars.ForEach(p => p.StickerPrice -= 3000);
			cars.ForEach(p => Console.WriteLine("{0} {1:C}", p.Model, p.StickerPrice));

			//creating new collection
			var newCars = from car in cars
						  where car.Model == "BMW"
						  select new { car.Make, car.Year };

			Console.ReadLine();

		}

		static void printSet(IEnumerable<Car> cars)
		{
			foreach(Car car in cars)
			{
				Console.WriteLine("{0} {1}", car.Model, car.VIN);
			}
			Console.WriteLine("---------------------");
		}

	}

	class Car
	{
		public string VIN { get; set; }
		public string Make { get; set; }
		public string Model { get; set; }
		public int Year { get; set; }
		public double StickerPrice { get; set; }
	}
}
