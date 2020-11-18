using Ay;

namespace Example
{
    class Program
    {
        static void Main(string[] args)
        {
            using Window window = new Window(1280, 900);
            Context ctx = window.Context;

            while (window.IsOpen()) {
                ctx.Clear();
                ctx.Viewport(0, 0, 1280, 900);

                ctx.UIBegin();

                ctx.UIEnd();
            }
        }
    }
}
