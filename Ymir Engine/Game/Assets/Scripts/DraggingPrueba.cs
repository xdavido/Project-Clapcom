using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using YmirEngine;


public class DraggingPrueba : YmirComponent
{

    public GameObject hola = null;

        public void Start()
        {
        Debug.Log("[ERROR] AAA: ");
        Debug.Log("[ERROR] Name: " + hola.Name);
        }
    public void Update()
    {
        Debug.Log("[ERROR] Name: " + hola.Name);
    }
}   

