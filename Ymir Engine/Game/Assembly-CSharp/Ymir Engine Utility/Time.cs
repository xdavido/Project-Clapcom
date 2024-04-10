using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace YmirEngine
{
    public class Time
    {

        public static extern float deltaTime
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }
        /// <summary>
        /// Returns the game timer in seconds
        /// </summary>
        public static extern float time
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

    }
}
