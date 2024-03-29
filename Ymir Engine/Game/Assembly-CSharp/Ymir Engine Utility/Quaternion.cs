using System;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using System.IO;

namespace YmirEngine
{

    [StructLayout(LayoutKind.Sequential)]
    public partial class Quaternion /*: IEquatable<Vector3>*/
    {

        public float x;
        public float y;
        public float z;
        public float w;

        public float this[int index] //Unity does this, is not the fastest way but maybe it's the way to fix the transform bug?
        {
            get
            {
                switch (index)
                {
                    case 0: return x;
                    case 1: return y;
                    case 2: return z;
                    case 3: return w;
                    default:
                        throw new IndexOutOfRangeException("Invalid Quaternion index!");
                }
            }

            set
            {
                switch (index)
                {
                    case 0: x = value; break;
                    case 1: y = value; break;
                    case 2: z = value; break;
                    case 3: w = value; break;
                    default:
                        throw new IndexOutOfRangeException("Invalid Quaternion index!");
                }
            }
        }

        public Quaternion(float x, float y, float z, float w)
        {
            this.x = x; this.y = y; this.z = z; this.w = w;
        }

        static readonly Quaternion identityQuaternion = new Quaternion(0F, 0F, 0F, 1F);
        public static Quaternion identity
        {
            get
            {
                return identityQuaternion;
            }
        }

        public void Set(float newX, float newY, float newZ, float newW)
        {
            x = newX; y = newY; z = newZ; w = newW;
        }

        //Rotate an angle(radiants) aroun an axis
        public static Quaternion RotateAroundAxis(Vector3 axis, float angle)
        {
            Quaternion ret = Quaternion.identity;

            float factor = (float)Math.Sin(angle / 2.0);

            ret.x = axis.x * factor;
            ret.y = axis.y * factor;
            ret.z = axis.z * factor;

            ret.w = (float)Math.Cos(angle / 2.0);

            //InternalCalls.CSLog(ret);
            //ret = ret.normalized;

            return ret;
        }

        public static float Dot(Quaternion a, Quaternion b)
        {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        public static Quaternion operator *(Quaternion quaternion, float scalar)
        {
            return new Quaternion(
                quaternion.x * scalar,
                quaternion.y * scalar,
                quaternion.z * scalar,
                quaternion.w * scalar);
        }

        public static Quaternion Normalize(Quaternion q)
        {
            float mag = (float)Math.Sqrt(Dot(q, q));

            if (mag < float.Epsilon)
                return Quaternion.identity;

            return new Quaternion(q.x / mag, q.y / mag, q.z / mag, q.w / mag);
        }

        public void Normalize()
        {
            Quaternion norm = Normalize(this);
            x = norm.x;
            y = norm.y;
            z = norm.z;
            w = norm.w;
        }

        public Quaternion normalized
        {
            get
            {
                return Normalize(this);
            }
        }

        public static Quaternion operator *(Quaternion q1, Quaternion q2)
        {
            return new Quaternion(
                q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
                q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
                q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
                q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
        }
        public static Quaternion operator +(Quaternion a, Quaternion b)
        {
            return new Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
        }
        public static Quaternion operator -(Quaternion a, Quaternion b)
        {
            return new Quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
        }
        public static Quaternion operator *(float scalar, Quaternion quaternion)
        {
            return new Quaternion(scalar * quaternion.x, scalar * quaternion.y, scalar * quaternion.z, scalar * quaternion.w);
        }
        public override string ToString()
        {
            return x.ToString() + ", " + y.ToString() + ", " + z.ToString() + ", " + w.ToString();
        }

        public static Quaternion Euler(float x, float y, float z)
        {
            // Convertimos los ángulos de grados a radianes
            float angleX = x * (float)Math.PI / 180.0f;
            float angleY = y * (float)Math.PI / 180.0f;
            float angleZ = z * (float)Math.PI / 180.0f;

            // Calculamos los senos y cosenos de la mitad de los ángulos
            float halfAngleX = angleX * 0.5f;
            float halfAngleY = angleY * 0.5f;
            float halfAngleZ = angleZ * 0.5f;

            float sinHalfX = (float)Math.Sin(halfAngleX);
            float cosHalfX = (float)Math.Cos(halfAngleX);
            float sinHalfY = (float)Math.Sin(halfAngleY);
            float cosHalfY = (float)Math.Cos(halfAngleY);
            float sinHalfZ = (float)Math.Sin(halfAngleZ);
            float cosHalfZ = (float)Math.Cos(halfAngleZ);

            // Calculamos los componentes del quaternion resultante
            float qx = cosHalfX * cosHalfY * cosHalfZ + sinHalfX * sinHalfY * sinHalfZ;
            float qy = sinHalfX * cosHalfY * cosHalfZ - cosHalfX * sinHalfY * sinHalfZ;
            float qz = cosHalfX * sinHalfY * cosHalfZ + sinHalfX * cosHalfY * sinHalfZ;
            float qw = cosHalfX * cosHalfY * sinHalfZ - sinHalfX * sinHalfY * cosHalfZ;

            return new Quaternion(qx, qy, qz, qw);
        }

        // Helper method to convert Quaternion to Euler angles in degrees
        private static Vector3 QuaternionToEulerAngles(Quaternion q)
        {
            // Convert Quaternion to Euler angles in radians
            float pitch = (float)Math.Atan2(2 * (q.y * q.z + q.w * q.x), 1 - 2 * (q.x * q.x + q.y * q.y));
            float yaw = (float)Math.Asin(2 * (q.w * q.y - q.x * q.z));
            float roll = (float)Math.Atan2(2 * (q.x * q.y + q.w * q.z), 1 - 2 * (q.y * q.y + q.z * q.z));

            // Convert radians to degrees and return as Vector3
            return new Vector3(pitch * 57.29578f, yaw * 57.29578f, roll * 57.29578f);
        }

        public static Quaternion RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta)
        {
            float dot = Quaternion.Dot(from, to);

            // If dot is negative, quaternions are more than 90 degrees apart
            if (dot < 0.0f)
            {
                // Flip one of the quaternions to move past the shortest path
                from = from * new Quaternion(0, 0, 0, -1.0f);
                dot = -dot;
            }

            // Clamp the dot product to be within [-1, 1] to avoid floating point errors
            dot = Math.Min(1.0f, Math.Max(-1.0f, dot));

            // Calculate the angle between the two quaternions
            float angle = (float)Math.Acos(dot) * 57.29578f; // Convert from radians to degrees

            // If the angle is very small, just return the 'to' quaternion
            if (angle < 0.001f)
                return to;

            // Calculate the fraction of the angle to rotate
            float t = Math.Min(1.0f, maxDegreesDelta / angle);

            // Interpolate between the two quaternions using spherical linear interpolation (slerp)
            Quaternion sinFrom = new Quaternion(from.x * (float)Math.Sin((1.0f - t) * angle), from.y * (float)Math.Sin((1.0f - t) * angle), from.z * (float)Math.Sin((1.0f - t) * angle), from.w * (float)Math.Sin((1.0f - t) * angle));
            Quaternion sinTo = new Quaternion(to.x * (float)Math.Sin(t * angle), to.y * (float)Math.Sin(t * angle), to.z * (float)Math.Sin(t * angle), to.w * (float)Math.Sin(t * angle));
            Quaternion result = Quaternion.Normalize(sinFrom * (float)Math.Sin((1.0f - t) * angle) + sinTo * (float)Math.Sin(t * angle));


            return result;
        }

        public static Quaternion Slerp(Quaternion a, Quaternion b, float t)
        {
            // Calcula el coseno del ángulo entre los dos cuaterniones
            float dot = Dot(a, b);

            // Si los cuaterniones son casi opuestos, selecciona uno de ellos y realiza una interpolación lineal
            if (dot < 0.0f)
            {
                b = new Quaternion(-b.x, -b.y, -b.z, -b.w);
                dot = -dot;
            }

            // Define el límite para evitar la división por cero
            const float DOT_THRESHOLD = 0.9995f;
            if (dot > DOT_THRESHOLD)
            {
                // Si los cuaterniones son casi iguales, selecciona la interpolación lineal
                Quaternion result = Normalize(a + t * (b - a));
                return result;
            }

            // Clampea el coseno para evitar errores de punto flotante
            dot = Math.Max(-1.0f, Math.Min(dot, 1.0f));

            // Calcula el ángulo entre los dos cuaterniones
            float theta_0 = (float)Math.Acos(dot);
            float theta = theta_0 * t;

            // Calcula la fracción para cada cuaternión
            Quaternion qperp = Normalize(b - a * dot);
            return a * (float)Math.Cos(theta) + qperp * (float)Math.Sin(theta);
        }

        public static Quaternion AngleAxis(float angle, Vector3 axis)
        {
            // Convert the angle to radians
            float radians = angle * Mathf.Deg2Rad;

            // Calculate half angle and its sin
            float halfAngle = radians * 0.5f;
            float sinHalfAngle = (float)Math.Sin(halfAngle);

            // Construct the quaternion
            Quaternion result = new Quaternion(
                axis.x * sinHalfAngle,
                axis.y * sinHalfAngle,
                axis.z * sinHalfAngle,
                (float)Math.Cos(halfAngle)
            );

            return result;
        }

    }
}