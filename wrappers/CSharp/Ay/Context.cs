using System;
using System.Runtime.InteropServices;

namespace Ay
{
    public class Context
    {
        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static string contextGetVersion(IntPtr context);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static string contextGetVendor(IntPtr context);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextClear(IntPtr context, float r, float g, float b, float a);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextViewport(IntPtr context, int x, int y, int w, int h);
        
        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextUiBegin(IntPtr context);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextUiEnd(IntPtr context);

        private IntPtr instance;
        public Window Window { get; private set; }

        public Context(IntPtr instance, Window window)
        {
            this.instance = instance;
            Window = window;
        }

        public string GetVersion()
        {
            return contextGetVersion(instance);
        }

        public string GetVendor()
        {
            return contextGetVendor(instance);
        }

        public void Clear(float r = 0f, float g = 0f, float b = 0f, float a = 1f)
        {
            contextClear(instance, r, g, b, a);
        }

        public void Viewport(int x, int y, int w, int h)
        {
            contextViewport(instance, x, y, w, h);
        }

        public void UIBegin()
        {
            contextUiBegin(instance);
        }

        public void UIEnd()
        {
            contextUiEnd(instance);
        }
    }
}