using System;
using System.Runtime.InteropServices;

namespace Ay
{
    public class Color
    {
        #region Structs
        [StructLayout(LayoutKind.Sequential)]
        public struct _Color
        {
            public float r;
            public float g;
            public float b;
            public float a;
        }
        #endregion

        #region Imports
        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static IntPtr colorCreate(float r, float g, float b, float a);

        [DllImport(Lib.Name, CharSet = CharSet.Unicode)]
        private extern static IntPtr colorCreateFromHex(UInt32 hex);
        #endregion

        #region Member Properties
        public _Color Instance => Marshal.PtrToStructure<_Color>(Ptr);
        public IntPtr Ptr { get; private set; }
        public float R => Instance.r;
        public float G => Instance.g;
        public float B => Instance.b;
        public float A => Instance.a;
        #endregion

        #region Class Properties
        public static Color Black => new Color(0x000000ff);
        public static Color White => new Color(0xffffffff);
        public static Color Red => new Color(0xff0000ff);
        public static Color Green => new Color(0x00ff00ff);
        public static Color Blue => new Color(0x0000ffff);
        #endregion

        public Color(float r, float g, float b, float a = 1f)
        {
            Ptr = colorCreate(r, g, b, a);
        }

        public Color(UInt32 hex)
        {
            Ptr = colorCreateFromHex(hex);
        }
    }
}