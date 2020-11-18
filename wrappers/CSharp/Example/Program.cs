using System;
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
            Color clearColor = Color.Black;

            var oldTime = DateTime.Now;

            while (window.IsOpen) {
                ctx.Clear(clearColor);
                ctx.Viewport(0, 0, 1280, 900);
                
                var currentTime = DateTime.Now;
                var elapsedTime = (currentTime - oldTime).TotalSeconds;
                oldTime = currentTime;

                ctx.UIBegin();
                ctx.UICreateWindow("Informations", () => {
                    ctx.UICreateText($"FPS: {(1.0 / elapsedTime).ToString("0.00")}", Color.White);
                    ctx.UICreateColorEditor("Screen color", ref clearColor);
                });
                ctx.UIEnd();
            }
        }
    }
}
