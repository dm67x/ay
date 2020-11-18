using System;
using System.Runtime.InteropServices;

namespace Ay
{
    public class Context
    {
        #region Delegates
        public delegate void DrawUI();
        #endregion

        #region Enums
        public enum ImGuiWindowFlags {
            None = 0,
            NoTitleBar = 1 << 0,
            NoResize = 1 << 1,
            NoMove = 1 << 2,
            NoScrollbar = 1 << 3,
            NoScrollWithMouse = 1 << 4,
            NoCollapse = 1 << 5,
            AlwaysAutoResize = 1 << 6,
            NoBackground = 1 << 7,
            NoSavedSettings = 1 << 8,
            NoMouseInputs = 1 << 9,
            MenuBar = 1 << 10,
            HorizontalScrollbar = 1 << 11,
            NoFocusOnAppearing = 1 << 12,
            NoBringToFrontOnFocus = 1 << 13,
            AlwaysVerticalScrollbar = 1 << 14,
            AlwaysHorizontalScrollbar = 1 << 15,
            AlwaysUseWindowPadding = 1 << 16,
            NoNavInputs = 1 << 18,
            NoNavFocus = 1 << 19,
            UnsavedDocument = 1 << 20,
            NoNav = ImGuiWindowFlags.NoNavInputs | ImGuiWindowFlags.NoNavFocus,
            NoDecoration = ImGuiWindowFlags.NoTitleBar | ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoScrollbar | ImGuiWindowFlags.NoCollapse,
            NoInputs = ImGuiWindowFlags.NoMouseInputs | ImGuiWindowFlags.NoNavInputs | ImGuiWindowFlags.NoNavFocus
        }
        #endregion

        #region Imports
        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static string contextGetVersion(IntPtr context);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static string contextGetVendor(IntPtr context);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextClear(IntPtr context, IntPtr color);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextViewport(IntPtr context, int x, int y, int w, int h);
        
        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextUiBegin(IntPtr context);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextUiEnd(IntPtr context);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextUiCreateWindow(IntPtr context, [MarshalAs(UnmanagedType.LPStr)] string name, DrawUI draw, ImGuiWindowFlags flags, float posX, float posY, float sizeX, float sizeY);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextUiCreateText(IntPtr context, IntPtr color, [MarshalAs(UnmanagedType.LPStr)] string fmt);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static void contextUiCreateColorEditor(IntPtr context, IntPtr color, [MarshalAs(UnmanagedType.LPStr)] string fmt);
        #endregion

        #region Member Attributes
        private IntPtr instance;
        #endregion

        #region Member Properties
        public Window Window { get; private set; }
        public string Version => contextGetVersion(instance);
        public string Vendor => contextGetVendor(instance);
        #endregion

        public Context(IntPtr instance, Window window)
        {
            this.instance = instance;
            Window = window;
        }

        public void Clear(Color color)
        {
            IntPtr colorPtr = Marshal.AllocHGlobal(Marshal.SizeOf(color.Instance));
            Marshal.StructureToPtr(color.Instance, colorPtr, true);
            contextClear(instance, colorPtr);
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

        public void UICreateWindow(string name, DrawUI method, ImGuiWindowFlags flags, Vector2 position, Vector2 size)
        {
            contextUiCreateWindow(instance, name, method, flags, position.x, position.y, size.x, size.y);
        }

        public void UICreateWindow(string name, DrawUI method, ImGuiWindowFlags flags, Vector2 position)
        {
            contextUiCreateWindow(instance, name, method, flags, position.x, position.y, 0, 0);
        }

        public void UICreateWindow(string name, DrawUI method, ImGuiWindowFlags flags)
        {
            contextUiCreateWindow(instance, name, method, flags, 0, 0, 0, 0);
        }

        public void UICreateWindow(string name, DrawUI method)
        {
            contextUiCreateWindow(instance, name, method, ImGuiWindowFlags.None, 0, 0, 0, 0);
        }

        public void UICreateText(string text, Color color)
        {
            IntPtr colorPtr = Marshal.AllocHGlobal(Marshal.SizeOf(color.Instance));
            Marshal.StructureToPtr(color.Instance, colorPtr, true);
            contextUiCreateText(instance, colorPtr, text);
        }

        public void UICreateColorEditor(string text, ref Color color)
        {
            contextUiCreateColorEditor(instance, color.Ptr, text);
        }
    }
}