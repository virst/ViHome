using System;
using System.Collections.Generic;
using System.Text;

namespace ViHome
{
    public class DevParameter
    {
        public int Id { get; set; }
        public ParameterDirection Direction { get; set; }
        public Parameter Parameter { get; set; }

        public readonly StringProperty ConfigurationDict = new StringProperty();
        public string Configuration
        {
            get
            {
                return ConfigurationDict.ToString();
            }
            set
            {
                ConfigurationDict.FromString(value);
            }
        }

        public int Period { get; set; }
    }
}
