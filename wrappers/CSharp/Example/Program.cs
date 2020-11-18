using Ay;
using GlmNet;

namespace Example
{
    class Program
    {
        static void Main(string[] args)
        {
            using Window window = new Window(1280, 900);
            Context ctx = window.Context;
            Color lightColor = Color.White;

            while (window.IsOpen()) {
                ctx.Clear(Color.Black);
                ctx.Viewport(0, 0, 1280, 900);

                ctx.UIBegin();
                ctx.UICreateWindow("info", () => {
                    ctx.UICreateText("FPS: ", Color.Red);
                    ctx.UICreateColorEditor("Light color", ref lightColor);
                });
                ctx.UIEnd();
            }
        }
    }
}
