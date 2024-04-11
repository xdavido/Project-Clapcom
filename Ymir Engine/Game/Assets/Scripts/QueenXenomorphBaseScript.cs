using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public enum QueenState
{
	//PHASE 1

	IDLE_PHASE_1,
	ACID_SHOT,

	//PHASE 2 AND 3
	//MOVEMENT

	IDLE_PHASE_2,
	WALKING_TO_PLAYER,
    WALK_BACKWARDS,
	WALKING_SIDEWAYS,

	//ATTACKS

    ACID_SPIT,
	CLAW,
	AXE_TAIL,
	PREPARE_DASH,
	DASH
}

enum Phase
{
	PHASE1,
	PHASE2,
	PHASE3
}

public class QueenXenomorphBaseScript : YmirComponent
{
    public GameObject thisReference = null;

    private QueenState queenState;

	

	public float life;

	public float speed;

    public float armor = 0;

    public float xSpeed = 0, ySpeed = 0;

	private float queenRotationSpeed;

    public float DetectionRadius = 200f;

    private GameObject player;

    //For attacks
    //Random attack
    private Random random = new Random();
    private bool randomSelected = false;
    private float selectedAttack = 0f;
    private float randomCounter = 0f;

    //Random movement
    private bool randomMovSelected = false;
    private float selectedMovement = 0f;
    private float sidewaysDuration = 0f;
    private float sidewaysTimer = 0f;

    private Vector3 vectorToPlayer = null;

    private int baseAttacks = 0;

    private float clawRadius = 30f;
    private float axeRadius = 50f;
    private float dashRadius = 50f;
    private float acidSpitRadius = 80f;

    private float clawAttackCooldown = 4f;
    private float clawTimer;
    private bool clawReady;
    private float clawAniCounter = 0f;
    private float clawAniDuration = 2.5f;

    private float acidSpitAttackCooldown = 20f;
    private float acidSpitTimer;
    private bool acidSpitReady;
    private float acidSpitAniCounter = 0f;
    private float acidSpitAniDuration = 2f;

    private float axeAttackCooldown = 18f;
	private float axeTimer;
	private bool axeReady;
    private float axeAniCounter = 0f;
    private float axeAniDuration = 3.5f;

    private float dashAttackCooldown = 22f;
    private float dashTimer;
	private bool dashReady;
    private float dashAniCounter1 = 0f;
    private float dashAniDuration1 = 2.5f;
    private float dashAniCounter2 = 0f;
    private float dashAniDuration2 = 2f;
    private bool dashDone = false;
    private float dashNum = 0f;

    private float backwardsTimer;
    private float backwardsDuration = 1f;



    public void Start()
	{
		//Temporary until we know for sure
		queenState = QueenState.IDLE_PHASE_2;
		
		life = 10000;
		speed = 12f;
		queenRotationSpeed = 2f;
        player = InternalCalls.GetGameObjectByName("Player");
		axeTimer = axeAttackCooldown;
		dashTimer = dashAttackCooldown;
        clawTimer = clawAttackCooldown;
        acidSpitTimer = 5f;
        clawReady = true;
        axeReady = false;
        dashReady = false;
        acidSpitReady = false;

        //Make sure it starts by walking towards the player
        selectedMovement = 1;
        randomMovSelected = true;

    }

    public void Update()
	{
        //Dont rotate while doing dash
        if (queenState != QueenState.DASH)
        {
            RotateQueen();
        }

        //Each attack has cooldown so its not spammed
        ClawCooldown();

        AcidSpitCooldown();

		//Once 2 attacks done, can use other attacks
		if (baseAttacks >= 2)
		{
			AxeCooldown();
			DashCooldown();
		}

		switch (queenState)
		{
			case QueenState.IDLE_PHASE_1:

            break;
			case QueenState.ACID_SHOT: 

			break;
			case QueenState.IDLE_PHASE_2:

                if (!randomMovSelected)
                {
                    selectedMovement = random.Next(0, 4);
                    randomMovSelected = true;
                }

                if ((CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, DetectionRadius)) && (!CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, clawRadius)))
                {
                    if (selectedMovement < 2)
                    {
                        Debug.Log("[ERROR] BOSS STATE WALKING TO PLAYER");
                        randomMovSelected = false;
                        queenState = QueenState.WALKING_TO_PLAYER;
                    }
                    else
                    {
                        Debug.Log("[ERROR] BOSS STATE WALKING SIDEWAYS");
                        randomMovSelected = false;
                        sidewaysDuration = random.Next(1, 4);
                        sidewaysTimer = 0f;
                        queenState = QueenState.WALKING_SIDEWAYS;
                    }
                }
                else
                {
                    CheckAttackDistance();
                }

                break;
			case QueenState.WALKING_TO_PLAYER:

                vectorToPlayer = player.transform.globalPosition - gameObject.transform.globalPosition;
                vectorToPlayer = Vector3.Normalize(vectorToPlayer);

                xSpeed = vectorToPlayer.x;
                ySpeed = vectorToPlayer.z;

                gameObject.SetVelocity(gameObject.transform.GetForward() * speed * 2);

                CheckAttackDistance();

                break;
			case QueenState.WALKING_SIDEWAYS:

                if (selectedMovement == 2)
                {
                    //Walk to the right side
                    gameObject.SetVelocity(new Vector3(-(gameObject.transform.GetForward().z * speed * 2), 0, (gameObject.transform.GetForward().x * speed * 2)));
                }
                else
                {
                    //Walk to the left side
                    gameObject.SetVelocity(new Vector3((gameObject.transform.GetForward().z * speed * 2), 0, -(gameObject.transform.GetForward().x * speed * 2)));
                }

                sidewaysTimer += Time.deltaTime;

                if (sidewaysTimer >= sidewaysDuration)
                {
                    CheckAttackDistance();

                    if (sidewaysTimer >= sidewaysDuration+2)
                    {
                        Debug.Log("[ERROR] BOSS STATE WALKING TO PLAYER");
                        queenState = QueenState.WALKING_TO_PLAYER;
                    }
                }

                break;
            case QueenState.WALK_BACKWARDS:

                gameObject.SetVelocity(gameObject.transform.GetForward() * -speed * 2);

                backwardsTimer += Time.deltaTime;

                if (backwardsTimer >= backwardsDuration)
                {
                    Debug.Log("[ERROR] BOSS STATE ACID SPIT");
                    backwardsTimer = 0f;
                    acidSpitReady = false;
                    acidSpitTimer = acidSpitAttackCooldown;
                    queenState = QueenState.ACID_SPIT;
                }

                break;
			case QueenState.CLAW:

                gameObject.SetVelocity(gameObject.transform.GetForward() * 0);

                clawAniCounter += Time.deltaTime;

                if (clawAniCounter >= clawAniDuration)
                {
                    Debug.Log("[ERROR] BOSS STATE IDLE");
                    clawAniCounter = 0f;
                    queenState = QueenState.IDLE_PHASE_2;
                }

            break;
            case QueenState.ACID_SPIT:

                gameObject.SetVelocity(gameObject.transform.GetForward() * 0);

                acidSpitAniCounter += Time.deltaTime;

                if (acidSpitAniCounter >= acidSpitAniDuration)
                {
                    Debug.Log("[ERROR] BOSS STATE IDLE");
                    acidSpitAniCounter = 0f;
                    baseAttacks++;
                    queenState = QueenState.IDLE_PHASE_2;
                }

                break;
			case QueenState.AXE_TAIL:

                gameObject.SetVelocity(gameObject.transform.GetForward() * 0);

                axeAniCounter += Time.deltaTime;

                if (axeAniCounter >= axeAniDuration)
                {
                    Debug.Log("[ERROR] BOSS STATE IDLE");
                    axeAniCounter = 0f;
                    queenState = QueenState.IDLE_PHASE_2;
                }

            break;
			case QueenState.PREPARE_DASH:

                gameObject.SetVelocity(gameObject.transform.GetForward() * 0);

                dashAniCounter1 += Time.deltaTime;

                if (dashAniCounter1 >= dashAniDuration1)
                {
                    Debug.Log("[ERROR] BOSS STATE DASH");
                    dashAniCounter1 = 0f;
                    queenState = QueenState.DASH;
                }

            break;
			case QueenState.DASH: 

				if (!dashDone)
                {
                    dashDone = true;
                    dashNum++;
                }
                gameObject.SetVelocity(gameObject.transform.GetForward() * speed * 8);

                dashAniCounter2 += Time.deltaTime;

                if (dashAniCounter2 >= dashAniDuration2)
                {
                    if (dashNum == 2)
                    {
                        Debug.Log("[ERROR] BOSS STATE IDLE");
                        dashAniCounter2 = 0f;
                        dashDone = false;
                        dashNum = 0;
                        queenState = QueenState.IDLE_PHASE_2;

                    }
                    else
                    {
                        Debug.Log("[ERROR] BOSS STATE PREPARE SECOND DASH");
                        dashAniCounter2 = 0f;
                        dashDone = false;
                        queenState = QueenState.PREPARE_DASH;
                    }
                }
            break;
		}

		//If player too far away stay idle
        if (!CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, DetectionRadius))
        {
            Debug.Log("[ERROR] BOSS STATE IDLE");

            queenState = QueenState.IDLE_PHASE_2;
        }

    }

    //GENERATE RANDOM NUMBER AMB PICK A ATTACK IF ITS COOLDOWN IS READY AND DISTANCE IS ENOUGH
    private void CheckAttackDistance()
    {
        if (!randomSelected)
        {
            selectedAttack = random.Next(0, 100);
            randomSelected = true;
        }

        randomCounter += Time.deltaTime;

        switch (selectedAttack)
        {
            case float i when (i <= 60):
                if ((CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, clawRadius)) && clawReady == true)
                {
                    Debug.Log("[ERROR] BOSS STATE CLAW");
                    clawReady = false;
                    clawTimer = clawAttackCooldown;
                    baseAttacks++;
                    queenState = QueenState.CLAW;
                }
                else
                {
                    //Reset counter
                    randomCounter = 0;
                    randomSelected = false;
                }

                break;
            case float i when (i > 60 && i <= 80):
                if ((CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, axeRadius)) && axeReady == true)
                {
                    Debug.Log("[ERROR] BOSS STATE AXE");
                    axeReady = false;
                    axeTimer = axeAttackCooldown;
                    queenState = QueenState.AXE_TAIL;
                }
                else
                {
                    //Reset counter
                    randomCounter = 0;
                    randomSelected = false;
                }

                break;
            case float i when (i > 80 && i <= 100):
                if ((CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, dashRadius)) && dashReady == true)
                {
                    Debug.Log("[ERROR] BOSS STATE PREPARE DASH");
                    dashReady = false;
                    dashTimer = dashAttackCooldown;
                    queenState = QueenState.PREPARE_DASH;
                }
                else
                {
                    //Reset counter
                    randomCounter = 0;
                    randomSelected = false;
                }

                break;
        }

        //Acid spit
        if ((CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, acidSpitRadius)) && (!CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, clawRadius)) && randomCounter == 0 && acidSpitReady == true)
        {
            Debug.Log("[ERROR] BOSS STATE ACID SPIT");
            acidSpitReady = false;
            acidSpitTimer = acidSpitAttackCooldown;
            queenState = QueenState.ACID_SPIT;

        }
        else if ((CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, clawRadius)) && acidSpitReady == true)
        {
            Debug.Log("[ERROR] BOSS STATE WALK BACKWARDS");
            queenState = QueenState.WALK_BACKWARDS;
        }
    }

    private void ClawCooldown()
    {
        if (clawTimer > 0 && !clawReady)
        {
            clawTimer -= Time.deltaTime;
            if (clawTimer <= 0)
            {
                clawReady = true;
            }
        }

    }

    private void AcidSpitCooldown()
    {
        if (acidSpitTimer > 0 && !acidSpitReady)
        {
            acidSpitTimer -= Time.deltaTime;
            if (acidSpitTimer <= 0)
            {
                acidSpitReady = true;
            }
        }

    }

    private void AxeCooldown()
	{
        if (axeTimer > 0 && !axeReady)
        {
            axeTimer -= Time.deltaTime;
            if (axeTimer <= 0)
            {
				axeReady = true;
            }
        }

    }

    private void DashCooldown()
	{
        if (dashTimer > 0 && !dashReady)
        {
            dashTimer -= Time.deltaTime;
            if (dashTimer <= 0)
            {
                dashReady = true;
            }
        }

    }

    private void RotateQueen()
    {
        Vector3 direction = player.transform.globalPosition - gameObject.transform.globalPosition;
        direction = direction.normalized;
        float angle = (float)Math.Atan2(direction.x, direction.z);

        //Debug.Log("Desired angle: " + (angle * Mathf.Rad2Deg).ToString());

        if (Math.Abs(angle * Mathf.Rad2Deg) < 1.0f)
            return;

        Quaternion dir = Quaternion.RotateAroundAxis(Vector3.up, angle);

        float rotationSpeed = Time.deltaTime * queenRotationSpeed;
        //Debug.Log("CS: Rotation speed: " + rotationSpeed.ToString());
        //Debug.Log("CS: Time: " + Time.deltaTime);

        Quaternion desiredRotation = Quaternion.Slerp(gameObject.transform.localRotation, dir, rotationSpeed);

        gameObject.transform.localRotation = desiredRotation;

        //Debug.Log("[ERROR] rotation:  " + gameObject.transform.localRotation);
    }

    public bool CheckDistance(Vector3 first, Vector3 second, float checkRadius)
    {
        float deltaX = Math.Abs(first.x - second.x);
        float deltaY = Math.Abs(first.y - second.y);
        float deltaZ = Math.Abs(first.z - second.z);

        return deltaX <= checkRadius && deltaY <= checkRadius && deltaZ <= checkRadius;
    }
    public QueenState GetState()
    {
        return queenState;
    }

}