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
        /// Set animation to loop when played. If not given <c>animationName</c> it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetLoop(object go, string animationName = "", bool loop = false);

        /// <summary>
        /// Set animation to play blackawards. If not given <c>animationName</c> it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetBackward(object go, string animationName = "", bool backwards = false);

        /// <summary>
        /// Set animation to ping-pong when played. If not given <c>animationName</c> it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetPingPong(object go, string animationName = "", bool pingPong = false);

        /// <summary>
        /// Set animation speed. If not given <c>animationName</c> it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetSpeed(object go, string animationName = "", float speed = 1.0f);

        /// <summary>
        /// Set animation to reset to zero when it ends. If not given <c>animationName</c> it applies to all animations in GameObject.        
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetResetToZero(object go, string animationName = "", bool resetToZero = true);

        /// <summary>
        /// Specify if <c>animationName</c> will blend  with <c>blendAnimation</c> when changing to it and the duration of the blend. If not given <c>animationName</c> it applies to all animations.
        /// <example>
        /// <para /> For exmaple:
        /// <code>
        /// <c>public void Start()</c> <para />
        /// { <para />
        /// <c>Animation.AddBlendOption(gameObject, "Walk", "Shoot", 5.0f);</c> <para />
        /// <c>Animation.AddBlendOption(gameObject, "Walk", "Dash", 2.5f);</c> <para />
        /// }
        /// </code>
        /// </example>
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void AddBlendOption(object go, string animationName  = "", string blendName = "", float transitionTime = 10.0f);

        /// <summary>
        /// Returns <c>true</c> if animation has ended, <c>false</c> if its currently playing
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool HasFinished(object go, string animationName);
    }
}
