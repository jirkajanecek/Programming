using System;
using System.Net;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyCodeLibrary
{
    public class Scrape
    {
        public string scrapeWebPage(
            string url
            )
        {
            return getWebPage(url); 
        }

        public string scrapeWebPage(
            string url,
            string filePath)
        {
            string content = getWebPage(url);
            File.WriteAllText(filePath, content);

            return content;
        }

        private string getWebPage(
            string url)
        {
            WebClient client = new WebClient();
            return client.DownloadString(url);
        }
    }
}
