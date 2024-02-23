using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace YmirEngine
{
    public class YmirComponent
    {

        public extern GameObject gameObject
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }


        public int UID;
    }
}
