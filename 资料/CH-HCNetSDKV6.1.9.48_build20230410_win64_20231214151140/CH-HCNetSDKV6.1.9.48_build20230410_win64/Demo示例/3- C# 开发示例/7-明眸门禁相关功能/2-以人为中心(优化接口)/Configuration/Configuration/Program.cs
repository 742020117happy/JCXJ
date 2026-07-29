using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace Configuration
{
    static class Program
    {

//应用程序的主入口点。

        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Configuration());
        }
    }

    public class ResponseStatus
    {
        public string requestURL { get; set; }
        public string statusCode { get; set; }
        public string statusString { get; set; }
        public string subStatusCode { get; set; }
        public string errorCode { get; set; }
        public string errorMsg { get; set; }
    }
}
