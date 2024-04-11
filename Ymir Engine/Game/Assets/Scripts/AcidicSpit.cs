using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class AcidicSpit : YmirComponent
{
    bool start = true;

    public float maxDistance = 30.0f;
    public float speed = 130.0f;
    public float damage = 90.0f;
    float time = 0.0f;
    public bool piercing = false;
    private Vector3 firstPosition;
    public GameObject acidPuddle;
    private Vector3 vSpeed;

    public void Update()
    {
        if (start)
        {

            firstPosition = gameObject.transform.globalPosition;
            vSpeed = GetDirection() * speed;

            start = false;
        }

        time += Time.deltaTime;

        //Mover bala
        gameObject.SetVelocity(vSpeed);

        if (time > (maxDistance / speed))
        {
            KillBullet();
        }

        return;
    }

    public void OnCollisionEnter(GameObject other)
    {
        //if (other.Tag == "Wall" && start == false)
        //{
        //    Debug.Log("Collision with Wall!");
        //    KillBullet();
        //}
        //else if (other.Tag == "Enemy" && start == false)
        //{
        //    Debug.Log("Collision with Enemy!");
        //    //Hace daño al enemigo

        //    KillBullet();
        //}
        //else 
        //{
        //    KillBullet(); 
        //}
    }

    void KillBullet()
    {
        //Crear charco de acido.
        InternalCalls.CreateAcidPuddle("AcidPudle", gameObject.transform.globalPosition);

        InternalCalls.Destroy(gameObject);
    }

    private Vector3 GetDirection()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");
        if (gameObject != null)
        {
            return gameObject.transform.GetForward();
        }
        else return new Vector3(0, 0, 0);
    }

}