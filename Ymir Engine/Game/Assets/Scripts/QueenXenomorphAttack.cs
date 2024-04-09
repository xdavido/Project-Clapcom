using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class QueenXenomorphAttack : YmirComponent
{
    public GameObject thisReference = null;

    public GameObject queen;

    private float damageTimer;

    private float attackDamage;

    private GameObject player;

    private Health healthScript;

    public void Start()
    {
        damageTimer = 0f;
        attackDamage = 1f;
        player = InternalCalls.GetGameObjectByName("Player");
        healthScript = player.GetComponent<Health>();

    }

    public void Update()
    {
        damageTimer -= Time.deltaTime;

        gameObject.SetRotation(queen.transform.globalRotation);

        //Since there's a small bug before attacking which causes the sensor not to move, the conditional is done this way for now
        if (queen.GetComponent<QueenXenomorphBaseScript>().GetState() != QueenState.CLAW && queen.GetComponent<QueenXenomorphBaseScript>().GetState() != QueenState.AXE_TAIL)
        {
            gameObject.SetPosition(queen.transform.globalPosition);
        }
        else if (queen.GetComponent<QueenXenomorphBaseScript>().GetState() == QueenState.CLAW)
        {
            //gameObject.SetPosition(new Vector3(gameObject.transform.globalPosition.x, gameObject.transform.globalPosition.y, gameObject.transform.GetForward().z + 5));
            if (queen.GetComponent<QueenXenomorphBaseScript>().CheckDistance(gameObject.transform.globalPosition, queen.transform.globalPosition, 10f))
            {
                gameObject.SetVelocity(gameObject.transform.GetForward() * 100f);
            }
            else
            {
                gameObject.SetVelocity(gameObject.transform.GetForward() * 0f);
            }
        }
        else if (queen.GetComponent<QueenXenomorphBaseScript>().GetState() == QueenState.AXE_TAIL)
        {
            //gameObject.SetPosition(new Vector3(gameObject.transform.globalPosition.x, gameObject.transform.globalPosition.y, gameObject.transform.GetForward().z + 5));
            if (queen.GetComponent<QueenXenomorphBaseScript>().CheckDistance(gameObject.transform.globalPosition, queen.transform.globalPosition, 10f))
            {
                gameObject.SetVelocity(gameObject.transform.GetForward() * 100f);
            }
            else
            {
                gameObject.SetVelocity(gameObject.transform.GetForward() * 0f);
            }
        }

    }

    public void OnCollisionStay(GameObject other)
    {
        if (other.Name == "Player" && damageTimer <= 0)
        {
            //Debug.Log("[ERROR] HIT");
            damageTimer = 1.5f;
            healthScript.TakeDmg(attackDamage);
        }
    }

}