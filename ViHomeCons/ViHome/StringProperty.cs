using System;
using System.Collections.Generic;
using System.Text.Json;

namespace ViHome
{
    public class StringProperty : Dictionary<string, string>
    {
        public StringProperty() : base(StringComparer.OrdinalIgnoreCase)
        {
        
        }

        public override string ToString()
        {
           return JsonSerializer.Serialize(this);
        }

        public void FromString(string s)
        {
            Dictionary<string, string> tmp = JsonSerializer.Deserialize<Dictionary<string, string>>(s);
            this.Clear();
            foreach (var t in tmp)
                this[t.Key] = t.Value;
        }
    }
}
