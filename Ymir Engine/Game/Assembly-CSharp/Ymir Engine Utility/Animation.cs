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
        public static extern void SetLoop(object go, string animationName, bool loop);

        /// <summary>
        /// Set animation to play blackawards. If not given <c>animationName</c> it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetBackward(object go, string animationName, bool backwards);

        /// <summary>
        /// Set animation to ping-pong when played. If not given <c>animationName</c> it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetPingPong(object go, string animationName, bool pingPong);

        /// <summary>
        /// Set animation speed. If not given <c>animationName</c> it applies to all animations in GameObject.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void SetSpeed(object go, string animationName, float speed);

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
    }
}
