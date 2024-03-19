﻿using System;
using YmirEngine;


public class BH_Bullet : YmirComponent
{
    public GameObject thisReference = null; //This is needed until i make all this be part of a component base class

    public float speed = 60.0f;
    public float maxLifeTime = 2.0f;
    public float currentLifeTime = 0.0f;
    public float yVel = 0.0f;

    private bool destroyed = false;

    public void Update()
    {
        thisReference = gameObject;
        currentLifeTime += Time.deltaTime;

        //thisReference.transform.localPosition += thisReference.GetForward() * (speed * Time.deltaTime);

        //Velocidad a la que se dispara la bala en el eje Z positivo (hacia delante)
        Vector3 vectorSpeed = new Vector3(0, 0, speed);

        //Se añade la velocidad al game object
        thisReference.SetVelocity(vectorSpeed);

        ////Caida de bala
        //yVel -= Time.deltaTime / 15.0f;
        //thisReference.transform.localPosition += (Vector3.up * yVel);

        if (currentLifeTime >= maxLifeTime && destroyed == false)
        {
            InternalCalls.Destroy(gameObject);
            destroyed = true;
        }
    }
}


