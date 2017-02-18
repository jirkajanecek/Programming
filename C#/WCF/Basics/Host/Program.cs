using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ServiceLibrary;
using System.ServiceModel;
using System.ServiceModel.Description;

namespace ServiceLibraryHost
{
	class Program
	{
		static void Main(string[] args)
		{
			//definition of base address
			Uri baseAddress = new Uri("http://localhost:8000/GettingStarted");

			//creating service host of our calculator
			ServiceHost host = new ServiceHost(typeof(Calculator), baseAddress);

			try
			{
				//adding service endpoint
				host.AddServiceEndpoint(typeof(ICalculator), new WSHttpBinding(), "Calculator");				

				//enable metadata exchange
				ServiceMetadataBehavior smb = new ServiceMetadataBehavior();
				smb.HttpGetEnabled = true;
				host.Description.Behaviors.Add(smb);

				//start service
				host.Open();
				Console.WriteLine("The Service was activated.");
				Console.WriteLine("Press <ENTER> to service termination.");
				Console.ReadLine();

				host.Close();
			}
			catch(CommunicationException ex)
			{
				Console.WriteLine("Exception in service: {0}", ex.Message);
				host.Abort();
			}
		}
	}
}
