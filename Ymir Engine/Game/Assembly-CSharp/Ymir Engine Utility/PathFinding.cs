using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Messaging;

namespace YmirEngine
{
    public class PathFinding : YmirComponent
    {
        public PathFinding()
        {       
            type = ComponentType.NAVMESHAGENT;
        }

        public extern float speed
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        public extern float angularSpeed
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }

        public extern float stoppingDistance
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern bool CalculateRandomPath(object startPos, float radius);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern bool CalculatePath(object startPos, object endPos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern bool IsPathPossible(object startPos, object endPos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetDestination();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetPointAt(int index);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern int GetPathIndex(object pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern int GetPathSize();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void ClearPath();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern Vector3 GetLastVector();




    }
}
