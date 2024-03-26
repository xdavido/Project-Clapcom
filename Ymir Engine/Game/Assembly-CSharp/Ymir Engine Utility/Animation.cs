using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace YmirEngine
{
    public class Animation
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void PlayAnimation(object go, string nameAnim);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void PauseAnimation(object go);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void ResumeAnimation(object go);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void StopAnimation(object go);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetLoop(object go, string nameAnim , bool loop);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetBackward(object go, string nameAnim, bool backwards);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetPingPong(object go, string nameAnim, bool pingPong);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetSpeed(object go, string nameAnim, float speed);
    }
}
