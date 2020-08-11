using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using ViHome;

namespace ViHomeApi
{
    public static class Utils
    {
        public readonly static ViHomeContext Db;

        static Utils()
        {
            Db = new ViHomeContext();
            if (!Db.NewDb) return;

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
            tmp.ConfigurationDict["ButtonMode-0"] = "0";
            tmp.Period = 500;

            d.Parameters.Add(tmp = new DevParameter() { Direction = ParameterDirection.In, Parameter = p });
            tmp.ConfigurationDict["Pin-0"] = "2";
            tmp.ConfigurationDict["Pin-1"] = "15";
            tmp.Period = 1000;

            p = new Parameter();
            p.SingleValue = false;
            p.Type = ParameterType.Numeric;
            p.ParameterName = "Погода";
            p.PropertiesDict["Count"] = "1";
            p.PropertiesDict["DevType"] = "ds18b20";

            d.Parameters.Add(tmp = new DevParameter() { Direction = ParameterDirection.Out, Parameter = p });
            tmp.ConfigurationDict["Pin-0"] = "12";
            tmp.Period = 60000;

            Db.Devices.Add(d);

            Db.SaveChanges();

        }
    }
}
