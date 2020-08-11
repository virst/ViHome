using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using Microsoft.EntityFrameworkCore;
using ViHome;
using static ViHomeApi.Utils;

namespace ViHomeApi.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class ParamValueController : ControllerBase
    {
        [HttpGet("get/{id}")]
        public string Get(int id)
        {
            return Db.GetValue(id) ??String.Empty;
        }

        [HttpGet("set/{id}")]
        public string Set(int id, [FromQuery]string val)
        {
            Db.SetValue(id, val);
            Db.SaveChanges();
            return "OK";
        }        
    }
}
