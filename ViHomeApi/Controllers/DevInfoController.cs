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
    public class DevInfoController : ControllerBase
    {
        [HttpGet("{id}")]
        public Device Get(int id)
        {
            return Db.Devices.Include(d => d.Parameters)
                             .ThenInclude(dp=> dp.Parameter)                             
                .FirstOrDefault(t => t.Id == id);
        }
    }
}
