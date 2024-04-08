using System;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

using System.Collections;
using System.IO;

namespace YmirEngine
{
    [StructLayout(LayoutKind.Sequential)]
    public partial class Vector2 //We use class because struct needs to be boxed and unboxed but class doesn't
    {
        //public bool Equals(Vector2 other)
        //{
        //    return (x == other.x && y == other.y);
        //}
        public float x;
        public float y;

        public float this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return x;
                    case 1: return y;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector2 index!");
                }
            }

            set
            {
                switch (index)
                {
                    case 0: x = value; break;
                    case 1: y = value; break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Vector2 index!");
                }
            }
        }

        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        public Vector2(float x, float y) { this.x = x; this.y = y; /*Debug.Log("Setting: " + x + ", " + y);*/ }
        //[MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Set(float newX, float newY, float newZ) { x = newX; y = newY;}

        public static Vector2 operator *(Vector2 a, float d) { return new Vector2(a.x * d, a.y * d); }
        public static Vector2 operator *(float d, Vector2 a) { return new Vector2(a.x * d, a.y * d); }
        public static Vector2 operator +(Vector2 a, Vector2 b) { return new Vector2(a.x + b.x, a.y + b.y); }
        public static Vector2 operator /(Vector2 a, float d) { return new Vector2(a.x / d, a.y / d); }
        public static Vector2 operator -(Vector2 a, float d) { return new Vector2(a.x - d, a.y - d); }
        public static Vector2 operator -(Vector2 a, Vector2 b) { return new Vector2(a.x - b.x, a.y - b.y); }

        static readonly Vector2 zeroVector = new Vector2(0F, 0F);
        static readonly Vector2 oneVector = new Vector2(1F, 1F);
        static readonly Vector2 upVector = new Vector2(0F, 1F);
        static readonly Vector2 downVector = new Vector2(1F, 0F);
        static readonly Vector2 positiveInfinityVector = new Vector2(float.PositiveInfinity, float.PositiveInfinity);
        static readonly Vector2 negativeInfinityVector = new Vector2(float.NegativeInfinity, float.NegativeInfinity);

        public static Vector2 zero { get { return zeroVector; } }
        public static Vector2 one { get { return oneVector; } }
        public static Vector2 up { get { return upVector; } }
        public static Vector2 down { get { return downVector; } }
        public static Vector2 positiveInfinity { get { return positiveInfinityVector; } }
        public static Vector2 negativeInfinity { get { return negativeInfinityVector; } }

        public static float Dot(Vector2 a, Vector2 b)
        {
            return (a.x * b.x + a.y + b.y);
        }

        public static float Magnitude(Vector2 vector) { return (float)Math.Sqrt(vector.x * vector.x + vector.y * vector.y); }
        public float magnitude { get { return (float)Math.Sqrt(x * x + y * y); } }

        public static Vector2 Normalize(Vector2 value)
        {
            float mag = Magnitude(value);
            if (mag > float.Epsilon)
                return value / mag;
            else
                return zero;
        }

        //public void Normalize()
        //{
        //    float mag = Magnitude(this);
        //    if (mag > float.Epsilon)
        //        this = this / mag;
        //    else
        //        this = zero;
        //}

        public Vector2 normalized { get { return Vector2.Normalize(this); } }


        public override string ToString()
        {
            return (this.x.ToString() + ", " + this.y.ToString());
        }

        public static float Distance(Vector2 a, Vector2 b)
        {
            return (a - b).magnitude;
        }

        public static Vector2 Lerp(Vector2 a, Vector2 b, float t)
        {
            t = (t < 0f) ? 0f : (t > 1f) ? 1f : t;
            return a * (1 - t) + b * t;
        }

        public static Vector2 SmoothDamp(Vector2 current, Vector2 target, ref Vector2 currentVelocity, float smoothTime, float maxSpeed = float.PositiveInfinity, float deltaTime = 0.02f)
        {
            // Ensure smooth time is non-negative
            smoothTime = Math.Max(0.0001f, smoothTime);

            // Calculate the smooth damp factor based on smooth time
            float omega = 2.0f / smoothTime;
            float x = omega * deltaTime;
            float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
            float smoothDampFactor = 1.0f - exp;

            // Calculate the difference between current and target vectors
            Vector2 difference = target - current;

            // Clamp the magnitude of the difference vector to ensure it doesn't exceed maxSpeed
            float maxDistanceDelta = maxSpeed * smoothTime;

            if (difference.magnitude > maxDistanceDelta)
            {
                difference = difference.normalized * maxDistanceDelta;
            }

            // Calculate the new velocity
            Vector2 newVelocity = currentVelocity + difference * omega * deltaTime;

            // Calculate the new position
            Vector2 newPosition = current + newVelocity * deltaTime;

            // Update currentVelocity
            currentVelocity = (1.0f - smoothDampFactor) * newVelocity;

            return newPosition;
        }

    }
}
