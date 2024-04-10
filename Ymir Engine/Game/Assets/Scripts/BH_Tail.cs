using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class BH_Tail : YmirComponent
{
    Player player;
    private float timer;
    private bool destroyed;

    public void Start()
	{
        //timer = 0;
        destroyed = false;

        GameObject playerObject = InternalCalls.GetGameObjectByName("Player");
        if (playerObject != null)
        {
            player = playerObject.GetComponent<Player>();

            timer = player.swipeTimer;
        }

    }

	public void Update()
	{
        timer = player.swipeTimer;

        if (player.swipeTimer <= 0 && !destroyed)
        {
            InternalCalls.Destroy(gameObject);
            destroyed = true;
        }
    }

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Enemy")
        {
            Debug.Log("Enemigo Colisionando");
            other.SetImpulse(other.transform.GetForward() * -1 * 60);
            
            //other.SetImpulse(new Vector3(0, 0, 1));
            //Le hace daño
        }
    }
}