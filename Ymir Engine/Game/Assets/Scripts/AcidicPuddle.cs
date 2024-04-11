using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class AcidPuddle : YmirComponent
{
    bool start = true;

    public bool enemy;
    public float damage = 25;
    int currentTicks = 0; //Guardamos los ticks actuales para esperar
    public float tickDamage = 0.3f;
    bool addTick; //No se si los triggers suceden todos a la vez pero por si acaso me esperare 1 update
    float timeTotal;
    public float duration = 1.8f;
    private GameObject player;

    public void Update()
    {
        if (start)
        {
            Debug.Log("ACID PUDDLE");

            //Game object del player para interactuar sobre el
            player = GetPlayer();
            start = false;
        }

        timeTotal += Time.deltaTime;
        if (addTick)
        {
            currentTicks++;
            addTick = false;
        }
        if (timeTotal > duration)
        {
            InternalCalls.Destroy(gameObject);
        }
    }

    public void OnCollisionEnter(GameObject other)
    {
        //if (other.Tag == "Enemy" && !enemy)
        //{
        //    //Deal damage
        //    //other.gameObject.GetComponent<DamageDetector>().HP -= damage;

        //    //Slow down

        //}
        //else if (other.Tag == "Player" && enemy) //Collision with player
        //{
        //    //Solo si no esta dashing recibir daño
        //    //if (!other.gameObject.GetComponent<MovementPlayer>().dashing)
        //    //{
        //    //    //Deal damage
        //    //    other.gameObject.GetComponent<DamageDetector>().HP -= damage;
        //    //}

        //}
        //else
        //{
        //    Debug.Log("No collide");
        //}
    }

    private GameObject GetPlayer()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");
        //Debug.Log(gameObject);
        if (gameObject != null)
        {
            return gameObject;
        }
        else return null;
    }

}