using System;
using System.Collections.Generic;
using System.Text;

namespace ViHome
{
    public class Device
    {
        public int Id { get; set; }
        public string DeviceName { get; set; }

        public List<DevParameter> Parameters { get; set; } = new List<DevParameter>();
    }
}
