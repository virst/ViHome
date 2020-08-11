using System;
using System.Collections.Generic;
using System.Text;

namespace ViHome
{
    public class ParameterValue
    {
        public readonly DateTime DefMoment = new DateTime(1900, 1, 1);

        public ParameterValue()
        {
            Moment = DefMoment;
        }

        public long Id { get; set; }
        public int ParameterId { get; set; }
        public DateTime Moment { get; set; }
        public string Val { get; set; }
    }
}
