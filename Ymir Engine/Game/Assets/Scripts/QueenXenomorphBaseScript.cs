using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

enum QueenState
{
	//PHASE 1

	IDLE_PHASE_1,
	ACID_SHOT,

	//PHASE 2 AND 3
	//MOVEMENT

	IDLE_PHASE_2,
	WALKING_TO_PLAYER,
	WALKING_SIDEWAYS,

	//ATTACKS

	CLAW,
	PREPARE_AXE_TAIL,
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

	private Phase phase;

	public float life;

	public float speed;

    public float armor = 0;

    public float xSpeed = 0, ySpeed = 0;

	private float queenRotationSpeed;

    private GameObject player;


    public void Start()
	{
		//Temporary until we know for sure
		queenState = QueenState.IDLE_PHASE_2;
		phase = Phase.PHASE2;
		life = 2000;
		speed = 12f;
		queenRotationSpeed = 2f;
        player = InternalCalls.GetGameObjectByName("Player");

    }

    public void Update()
	{

		RotateQueen();

		switch (queenState)
		{
			case QueenState.IDLE_PHASE_1:

			break;
			case QueenState.ACID_SHOT: 

			break;
			case QueenState.IDLE_PHASE_2:

			break;
			case QueenState.WALKING_TO_PLAYER: 
				
			break;
			case QueenState.WALKING_SIDEWAYS: 
				
			break;
			case QueenState.CLAW: 
				
			break;
			case QueenState.PREPARE_AXE_TAIL: 
				
			break;
			case QueenState.AXE_TAIL: 
				
			break;
			case QueenState.PREPARE_DASH: 
				
			break;
			case QueenState.DASH: 
				
			break;
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
}