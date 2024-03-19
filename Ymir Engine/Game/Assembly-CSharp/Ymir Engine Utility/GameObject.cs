using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace YmirEngine
{
    public sealed class GameObject
    {
        public string name;
        public UIntPtr pointer;
        public Transform transform;
        
        public GameObject()
        {
            name = "Empty";
            pointer = UIntPtr.Zero;
        }
        public GameObject(string _name, UIntPtr ptr, UIntPtr transPTR)
        {
            name = _name;
            pointer = ptr;
            transform = new Transform();
            transform.pointer = transPTR;

        }


        public T GetComponent<T>() //where T : DiamondComponent
        {
            //ComponentType type = T.get;
            ComponentType retValue = ComponentType.SCRIPT;
            if (YmirComponent.componentTable.ContainsKey(typeof(T)))
            {
                retValue = YmirComponent.componentTable[typeof(T)];
            }

            return TryGetComponent<T>(typeof(T).ToString(), (int)retValue);
        }


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern internal T TryGetComponent<T>(string type, int inputType = 0);


        public extern string Name
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        }
        public extern GameObject parent
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;
        } 
      
        public extern string Tag
        {
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            get;

            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            set;
        }





        int GetHash()
        {
            return this.GetHashCode();
        }

        // Try to fix duplicated methods
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //public extern Vector3 GetForward();

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //public extern Vector3 GetRight();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void SetVelocity(Vector3 velocity);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void SetImpulse(Vector3 impulse);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void AddComponent(int componentType);

    }
}
