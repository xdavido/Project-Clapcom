using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using YmirEngine;

namespace YmirEngine
{
    /// <summary>
    /// Represents the transformation of a game object.
    /// </summary>
    public class Transform : YmirComponent
    {
        /// <summary>
        /// Default constructor for the Transform class.
        /// </summary>
        public Transform()
        {
            type = ComponentType.TRANSFORM;
        }

        /// <summary>
        /// The local position of the transform.
        /// </summary>
        public extern Vector3 localPosition
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        /// <summary>
        /// The global position of the transform.
        /// </summary>
        public extern Vector3 globalPosition
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        /// <summary>
        /// The local rotation of the transform.
        /// </summary>
        public extern Quaternion localRotation
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        /// <summary>
        /// The global rotation of the transform.
        /// </summary>
        public extern Quaternion globalRotation
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        /// <summary>
        /// The local scale of the transform.
        /// </summary>
        public extern Vector3 localScale
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        /// <summary>
        /// The global scale of the transform.
        /// </summary>
        public extern Vector3 globalScale
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        /// <summary>
        /// Retrieves the forward direction of the transform in world space.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetForward();

        /// <summary>
        /// Retrieves the right direction of the transform in world space.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetRight();

        /// <summary>
        /// Retrieves the up direction of the transform in world space.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetUp();

        /// <summary>
        /// Retrieves the forward direction of the transform in local space.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetLocalForward();

        /// <summary>
        /// Retrieves the right direction of the transform in local space.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetLocalRight();

        /// <summary>
        /// Retrieves the up direction of the transform in local space.
        /// </summary>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetLocalUp();

        /// <summary>
        /// Makes the transform look at a specified point.
        /// </summary>
        /// <param name="pointToLook">The point to look at.</param>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void LookAt(Vector3 pointToLook);
    }
}
