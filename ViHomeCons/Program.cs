using System;
using System.IO;
using ViHome;

namespace ViHomeCons
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            
            try
            {
                if (File.Exists(ViHomeContext.Fn))
                    File.Delete(ViHomeContext.Fn);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            var db = new ViHomeContext();

            Device d = new Device();
            d.DeviceName = "WiFi-1";

            Parameter p = new Parameter();
            p.SingleValue = true;
            p.Type = ParameterType.Switch;
            p.ParameterName = "Свет";
            p.PropertiesDict["Count"] = "2";
            p.PropertiesDict["RollMode"] = "1";

            DevParameter tmp;

            d.Parameters.Add(tmp = new DevParameter() { Direction = ParameterDirection.Out, Parameter = p });
            tmp.ConfigurationDict["ButtonCount"] = "1";
            tmp.ConfigurationDict["Pin-0"] = "14";

            d.Parameters.Add(tmp = new DevParameter() { Direction = ParameterDirection.In, Parameter = p });
            tmp.ConfigurationDict["Pin-0"] = "9";
            tmp.ConfigurationDict["Pin-1"] = "10";

            p = new Parameter();
            p.SingleValue = false;
            p.Type = ParameterType.Numeric;
            p.ParameterName = "Погода";
            p.PropertiesDict["Count"] = "1";
            p.PropertiesDict["DevType"] = "ds18b20";

            d.Parameters.Add(tmp = new DevParameter() { Direction = ParameterDirection.Out, Parameter = p});
            tmp.ConfigurationDict["Pin-0"] = "12";

            db.Devices.Add(d);

            db.SaveChanges();
                         

        }
    }
}
