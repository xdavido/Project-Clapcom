using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

using YmirEngine;



enum EnemyState
{
    Idle,
    Moving,
    Attacking,
    Dead
}

enum WanderState
{
    REACHED,
    GOING,
    CHASING,
    STOPED
}
public class RandomPointGenerator
{
    private Random random = new Random();

    public (float x, float y) GetRandomPointInRadius(float radius)
    {
        // Generate random angle
        double angle = random.NextDouble() * 2 * Math.PI;

        // Calculate random point coordinates
        float x = (float)(Math.Cos(angle) * radius);
        float y = (float)(Math.Sin(angle) * radius);

        return (x, y);
    }
}

public class EnemyBehaviour : YmirComponent
{
	public GameObject thisReference = null;


    public float movementSpeed = 2f;

    public float life = 100f;

    private float wanderRadius = 10f;

    public float xSpeed = 0, ySpeed = 0;


    //public float wanderTimer = 5f;

    private GameObject player;

    public bool PlayerDetected = false;

    public float DetectionRadius = 15f;

    //private EnemyState state = EnemyState.Idle;

    private WanderState wanderState;

    RandomPointGenerator pointGenerator;    

    private Vector3 vectorToPlayer = null;

    private float wanderTimer;
    public float wanderDuration = 5f;

    private float stopedTimer;
    public float stopedDuration = 1f;

    public void Start()
    {
        pointGenerator = new RandomPointGenerator();
        wanderState = WanderState.REACHED;
        wanderDuration = 5f;
        wanderTimer = wanderDuration;
        player = InternalCalls.GetGameObjectByName("Player");
        movementSpeed = 2f;
        stopedDuration = 1f;
        DetectionRadius = 15f;
        wanderRadius = 10f;
    }

    public void Update()
	{



        switch (wanderState)
        {
            case WanderState.REACHED:
                (xSpeed, ySpeed) = pointGenerator.GetRandomPointInRadius(wanderRadius);
                wanderTimer = wanderDuration;
                Debug.Log("[ERROR] Current State: REACHED");
                wanderState = WanderState.GOING;
                movementSpeed = 2f;
                break;

            case WanderState.GOING:
                HandleRotation();
                Debug.Log("[ERROR] Current State: GOING");
                ProcessMovement();
                Debug.Log("[ERROR] Forward: " + gameObject.transform.GetForward());
                gameObject.SetVelocity(gameObject.transform.GetForward() * -movementSpeed);
                break;


            case WanderState.CHASING:

                Debug.Log("[ERROR] Current State: CHASING");
                vectorToPlayer = player.transform.globalPosition - gameObject.transform.globalPosition;
                vectorToPlayer = Vector3.Normalize(vectorToPlayer);
                
                xSpeed = vectorToPlayer.x;
                ySpeed = -vectorToPlayer.z;

                HandleRotation();

                gameObject.SetVelocity(gameObject.transform.GetForward() * -movementSpeed * 2);
                break;

            case WanderState.STOPED:
                Debug.Log("[ERROR] Current State: STOPED");
                ProcessStopped();
                break;
        }


        if (player.transform.globalPosition.x - gameObject.transform.globalPosition.x < DetectionRadius && player.transform.globalPosition.z - gameObject.transform.globalPosition.z < DetectionRadius)
        {
            movementSpeed = 2f;
            wanderState = WanderState.CHASING;
        }

    }

    private void ProcessStopped()
    {
        if (stopedTimer > 0)
        {
            stopedTimer -= Time.deltaTime;
            if (stopedTimer <= 0)
            {
                wanderState = WanderState.REACHED;
            }
        }
    }
    private void ProcessMovement()
    {
        if (wanderTimer > 0)
        {
            wanderTimer -= Time.deltaTime;
            if (wanderTimer <= 0)
            {

                Debug.Log("[ERROR] AAAA");
                movementSpeed = 0;
                stopedTimer = stopedDuration;
                wanderState = WanderState.STOPED;

            }
        }
    }

    private void HandleRotation()
    {

        //Vector3 directionToPlayer = (player.transform.globalPosition - gameObject.transform.globalPosition);



        //directionToPlayer = Vector3.Normalize(directionToPlayer);


        //Debug.Log("[ERROR] Direction: " + directionToPlayer);

        //if (directionToPlayer != Vector3.zero)
        //{
        //    float angle = Mathf.Atan2(directionToPlayer.x, directionToPlayer.z) * Mathf.Rad2Deg;

        //    Quaternion targetRotation = Quaternion.Euler(0, angle, 0);

        //    // Aplicar rotación al objeto, considerando su rotación actual
        //    gameObject.SetRotation( Quaternion.Euler(0,targetRotation.y,0));
        //}

        Vector3 aX = new Vector3(xSpeed, 0, ySpeed);
        aX = Vector3.Normalize(aX);

        //Debug.Log("[ERROR] Vector: " + aX);

        Quaternion targetRotation = Quaternion.identity;

        Vector3 aY = gameObject.transform.GetUp();
        //Debug.Log("[ERROR] Vector: " + aY);

        if (aX != Vector3.zero)
        {
            float angle = 0;

            if (aX.x >= 0)
            {
                angle = (float)Math.Acos(Vector3.Dot(aX, aY) - 1);
            }
            else if (aX.x < 0)
            {
                angle = -(float)Math.Acos(Vector3.Dot(aX, aY) - 1);
            }

            


            //Debug.Log("[ERROR] Angle: " + angle);

            targetRotation = Quaternion.AngleAxis(angle * Mathf.Rad2Deg, Vector3.up);

            //Debug.Log("[ERROR] Angle: " + targetRotation);
        }
        gameObject.SetRotation(targetRotation);

    }
}