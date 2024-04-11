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

    GameObject playerObject;

    public void Start()
	{
        //timer = 0;
        destroyed = false;

        playerObject = InternalCalls.GetGameObjectByName("Player");

        if (playerObject != null)
        {
            player = playerObject.GetComponent<Player>();

            timer = player.swipeTimer;
        }

    }

	public void Update()
	{
        timer = player.swipeTimer;

        gameObject.SetRotation(playerObject.transform.globalRotation);
        gameObject.SetPosition(playerObject.transform.globalPosition + (playerObject.transform.GetForward() * -2.5f));

        //gameObject.transform.globalPosition = playerObject.transform.globalPosition;

        if (player.swipeTimer <= 0 && !destroyed)
        {
            InternalCalls.Destroy(gameObject);
            destroyed = true;
        }
    }

    public void OnCollisionStay(GameObject other)
    {
        Debug.Log("Enemigo Colisionando");

        if (other.name == "Enemy")
        {
            //Debug.Log("Enemigo Colisionando");
            other.SetImpulse(other.transform.GetForward() * -1 * 60);
            
            //Le hace daño
        }
    }
}