using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;


namespace import {
    
    public class Import {
        
        [DllImport("/home/matyas/Documents/3Dengine/shared/lib3dvfunc.so", CallingConvention = CallingConvention.Cdecl)]
         public static extern IntPtr Element(byte vecnum);
        
        [DllImport("/home/matyas/Documents/3Dengine/shared/lib3dvfunc.so", CallingConvention = CallingConvention.Cdecl)]
        public static extern UInt64 Size(byte vecnum);
        
        [DllImport("/home/matyas/Documents/3Dengine/shared/lib3dvfunc.so", CallingConvention = CallingConvention.Cdecl)]
        public static extern void LoadOBJ(string path);
    }
}