using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace YmirEngine
{
    public static class Mathf
    {
        public static float Distance(Vector3 pointA, Vector3 pointB)
        {
            Vector3 distance = pointB - pointA;
            return distance.magnitude;
        }

        public static float Lerp(float from, float to, float t)
        {
            return (1.0f - t) * from + to * t;
        }

        //given 2 numbers and a number between them, inverse lerp returns a value between 0 and 1
        public static float InvLerp(float from, float to, float value)
        {
            return (value - from) / (to - from);
        }

        public static float LerpAngle(float from, float to, float t)
        {
            float delta = Repeat((to - from), 360);
            if (delta > 180)
                delta -= 360;
            return from + delta * Clamp01(t);
        }

        public static float Clamp01(float value)
        {
            if (value < 0f)
                return 0f;
            else if (value > 1f)
                return 1f;
            else
                return value;
        }

        public static float Clamp(float value, float min, float max)
        {
            if (value < min)
                value = min;
            else if (value > max)
                value = max;
            return value;
        }

        public static float Repeat(float t, float length)
        {
            return Clamp(t - Mathf.Floor(t / length) * length, 0.0f, length);
        }

        public static float Floor(float f) { return (float)Math.Floor(f); }

        public const float Rad2Deg = 57.29578f;
        public const float Deg2Rad = 0.0174532925199432957f;

        public const float PI = 3.14159265358979323846f;

        public static float Atan2(float y, float x)
        {
            float angle = Mathf.Atan(y / x);
            if (x < 0)
            {
                angle += Mathf.PI;
            }
            else if (y < 0)
            {
                angle += 2 * Mathf.PI;
            }
            return angle;
        }

        public static float Atan(float x)
        {
            return x - (x * x * x) / 3 + (x * x * x * x * x) / 5 - (x * x * x * x * x * x * x) / 7;
        }

        public static float Abs(float value)
        {
            return value < 0 ? -value : value;
        }

        public static float Epsilon()
        {
            return 0.000001f; // A very small float value
        }
        public static float Max(float a, float b)
        {
            return a > b ? a : b;
        }
    }
}
