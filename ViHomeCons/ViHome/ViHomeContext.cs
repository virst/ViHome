using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ViHome
{
    public class ViHomeContext : DbContext
    {
        public const string Fn = "ViHome.db";

        public DbSet<Device> Devices { get; set; }
        public DbSet<DevParameter> ParameterDirections { get; set; }
        public DbSet<Parameter> Parameters { get; set; }
        public DbSet<ParameterValue> ParameterValues { get; set; }

        public static readonly ILoggerFactory MyLoggerFactory
= LoggerFactory.Create(builder => { builder.AddFile("log.txt"); });

        protected override void OnConfiguring(DbContextOptionsBuilder options)
           => options
           .UseLoggerFactory(MyLoggerFactory)
           .UseSqlite("Data Source=" + Fn);

        public readonly bool NewDb;

        public ViHomeContext()
        {
            NewDb = Database.EnsureCreated();
        }

        public string GetValue(int ParamId)
        {
            var md = ParameterValues.Where(v => v.ParameterId == ParamId).Max(v => (DateTime?)v.Moment);
            if (md == null) return null;
            return ParameterValues.Where(v => v.ParameterId == ParamId && v.Moment == md.Value).Select(v => v.Val).FirstOrDefault();
        }

        public void SetValue(int ParamId, string val) => SetValue(ParamId, val, DateTime.Now);
        public void SetValue(int ParamId, string val, DateTime moment)
        {
            Parameter p = Parameters.FirstOrDefault(p => p.Id == ParamId);
            if (p == null) return;
            if (!p.SingleValue || ParameterValues.Where(v => v.ParameterId == ParamId).Count() == 0)
                p.Values.Add(new ParameterValue() { Moment = moment, Val = val });
            else
            {
                var pv = ParameterValues.First(v => v.ParameterId == ParamId);
                pv.Val = val;
                pv.Moment = moment;
            }

        }
    }
}
