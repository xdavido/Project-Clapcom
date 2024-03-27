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
        /// <summary>
        /// Plays an animation given its name from the start.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void PlayAnimation(object go, string animationName);

        /// <summary>
        /// Pauses the currently playing animation.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void PauseAnimation(object go);

        /// <summary>
        /// Resumes the currently playing animation.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void ResumeAnimation(object go);

        /// <summary>
        /// Stops the currently playing animation.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void StopAnimation(object go);

        /// <summary>
        /// Set animation to loop when played. If not given an animation name it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetLoop(object go, string animationName, bool loop);

        /// <summary>
        /// Set animation to play blackawards. If not given an animation name it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetBackward(object go, string animationName, bool backwards);

        /// <summary>
        /// Set animation to ping-pong when played. If not given an animation name it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetPingPong(object go, string animationName, bool pingPong);

        /// <summary>
        /// Set animation speed. If not given a name it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetSpeed(object go, string animationName, float speed);

        /// <summary>
        /// Specify which animations will blend into 'animationName' and the duration of the blend. If not given an animation name it applies to all animations.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void AddBlendOption(object go, string animationName  = "", string blendName = "", float transitionTime = 10.0f);
    }
}
