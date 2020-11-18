using System;
using System.Runtime.InteropServices;

namespace Ay
{
    public class Window : IDisposable
    {
        #region Imports
        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static IntPtr windowNew(int width, int height);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void windowDispose(IntPtr window);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static bool windowIsOpen(IntPtr window);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static IntPtr windowGetContext(IntPtr window);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void windowGetSize(IntPtr window, out int width, out int height);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static bool windowKeyPressed(IntPtr window, int key);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static bool windowKeyReleased(IntPtr window, int key);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void windowGetMousePosition(IntPtr window, out float x, out float y);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void windowClose(IntPtr window);
        #endregion

        #region Member Attributes
        private IntPtr instance;
        #endregion

        #region Member Properties
        public Context Context { get; private set; }
        public bool IsOpen => windowIsOpen(instance);
        public Tuple<int, int> Size {
            get {
                windowGetSize(instance, out int w, out int h);
                return Tuple.Create(w, h);
            }
        }
        public Tuple<float, float> MousePosition {
            get {
                windowGetMousePosition(instance, out float x, out float y);
                return Tuple.Create(x, y);
            }
        }
        #endregion

        public Window(int width, int height)
        {
            instance = windowNew(width, height);
            Context = new Context(windowGetContext(instance), this);
        }

        public void Dispose()
        {
            windowDispose(instance);
        }

        public bool IsKeyPressed(int key)
        {
            return windowKeyPressed(instance, key);
        }

        public bool IsKeyReleased(int key)
        {
            return windowKeyReleased(instance, key);
        }

        public void Close()
        {
            windowClose(instance);
        }
    }
}