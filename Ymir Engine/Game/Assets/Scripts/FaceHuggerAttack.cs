using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class FaceHuggerAttack : YmirComponent
{
    private float damageTimer, damageTime;

    private float timeSinceCollision;

    private float attackDamage;

    private GameObject player;

    private Health healthScript;

    public void Start()
	{
        damageTimer = 30f;
        damageTime = 0f;
        attackDamage = 3f;
        timeSinceCollision = 1f;
        player = InternalCalls.GetGameObjectByName("Player");
        healthScript = player.GetComponent<Health>();

    }

    public void Update()
	{
        //So that it resets after some small amount of time out of the collision
        if (damageTime > 0f)
        {
            timeSinceCollision++;
            if (timeSinceCollision - damageTime > 10)
            {
                timeSinceCollision = 1f;
                damageTime = 0f;
            }
        }
	}
    public void OnCollisionStay(GameObject other)
    {
        if (other.Name == "Player")
        {
            damageTime++;
            if (damageTime > damageTimer)
            {
                Debug.Log("[ERROR] HIT");
                healthScript.TakeDmg(attackDamage);
                damageTime = 0f;
            }
        }
    }
}