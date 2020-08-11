using System;
using System.Collections.Generic;
using System.Text;

namespace ViHome
{
    public class Parameter
    {
        public int Id { get; set; }

        public string ParameterName { get; set; }
        public ParameterType Type { get; set; }

        public bool SingleValue { get; set; }

        public readonly StringProperty PropertiesDict = new StringProperty();
        public string Properties
        {
            get
            {
                return PropertiesDict.ToString();
            }
            set
            {
                PropertiesDict.FromString(value);
            }
        }

        public List<ParameterValue> Values { get; set; } = new List<ParameterValue>();
    }
}
