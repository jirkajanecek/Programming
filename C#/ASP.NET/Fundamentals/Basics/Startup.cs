using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(Basics.Startup))]
namespace Basics
{
    public partial class Startup {
        public void Configuration(IAppBuilder app) {
            ConfigureAuth(app);
        }
    }
}
