using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class TP_Maps_Test : YmirComponent
{
    private Vector3 playerSpawnPosition = new Vector3(10, 10, 0);
    public void Start()
	{
			Debug.Log("HelloWorld"); 
	}

	public void Update()
	{
		return;
	}

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player")
        {

            other.GetComponent<Basic_Movment>().SetCoordinates(100f, 0f, 0f);//no va por ahora
            //other.SetPosition(new Vector3(10, 10, 0));
            InternalCalls.LoadScene("Assets/Door.yscene");
            other.GetComponent<Basic_Movment>().SetTeleport(true);



            return;

        }

    }
    //public void OnCollisionStay(GameObject other)
    //{
    //    if (other.Tag == "Player")
    //    {


    //        other.SetPosition(new Vector3(10, 10, 0));
    //        InternalCalls.LoadScene("Assets/NPC_Base.yscene");
    //        //other.SetPosition(new Vector3(10, 10, 0));


    //        return;

    //    }

    //}




}