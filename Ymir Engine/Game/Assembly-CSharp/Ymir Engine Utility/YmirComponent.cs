using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace YmirEngine
{
    public enum ComponentType
    {
        NONE = -1,

        TRANSFORM,
        MESH,
        MATERIAL,
        SCRIPT,
        AUDIO_SOURCE,
        AUDIO_LISTENER,
        CAMERA,
        RIGIDBODY,
        PHYSICS,
        ANIMATION,
        UI_TRAMSFORM,
        UI,
        LIGHT,
        NAVMESHAGENT
    }
    public class YmirComponent
    {       

        public UIntPtr pointer;
        public ComponentType type;
        public static Dictionary<System.Type, ComponentType> componentTable = new Dictionary<Type, ComponentType> {
            { typeof(Transform), ComponentType.TRANSFORM},
            { typeof(PathFinding), ComponentType.NAVMESHAGENT  },
        };


        public YmirComponent()
        {
            this.type = ComponentType.SCRIPT;
        }
        public extern GameObject gameObject
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }

        public ComponentType GetComponentType()
        {
            return type;
        }


        public int UID;
    }
}
