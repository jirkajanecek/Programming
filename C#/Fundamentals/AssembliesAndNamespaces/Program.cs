//namespaces
using System;
using System.IO;
using System.Net;

namespace AssembliesAndNamespaces
{
    class Program
    {
        static void Main(string[] args)
        {
            string fileName = @"D:\Temp\C#_test.txt";
            string[] lines = { "First line", "Second line", "Third line" };
            File.WriteAllLines(fileName, lines);
            string [] content = System.IO.File.ReadAllLines(fileName);
            
            WebClient webClient = new WebClient();
            string webContent = webClient.DownloadString("http://msdn.microsoft.com");
            Console.WriteLine(webContent);

            Console.ReadLine();

        }
    }
}
