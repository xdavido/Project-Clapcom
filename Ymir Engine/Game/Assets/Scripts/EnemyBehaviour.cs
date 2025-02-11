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
    HIT,
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


    public float movementSpeed = 20f;
    private float actualMovementSpeed;

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
    private Health healthScript;

    private Vector3 vectorToPlayer = null;

    private float wanderTimer;
    public float wanderDuration = 5f;

    private float stopedTimer;
    public float stopedDuration = 1f;

    private float cumTimer;
    public float cumDuration = 2f;

    private float cumTimer2;
    public float cumDuration2 = 5f;

    public void Start()
    {
        pointGenerator = new RandomPointGenerator();
        wanderState = WanderState.REACHED;
        wanderDuration = 5f;
        wanderTimer = wanderDuration;
        player = InternalCalls.GetGameObjectByName("Player");
        healthScript = player.GetComponent<Health>();
        movementSpeed = 10f;
        stopedDuration = 1f;
        DetectionRadius = 10f;
        wanderRadius = 10f;
        cumDuration = 2f;
        cumDuration2 = 5f;

        cumTimer = cumDuration2;

        gameObject.SetPosition(new Vector3(-181,4,230));

    }

    public void Update()
    {
   
        cumTimer2 -= Time.deltaTime;
        if(cumTimer2 <= 0)
        {
            switch (wanderState)
            {
                case WanderState.REACHED:
                    (xSpeed, ySpeed) = pointGenerator.GetRandomPointInRadius(wanderRadius);
                    wanderTimer = wanderDuration;
                    //Debug.Log("[ERROR] Current State: REACHED");
                    wanderState = WanderState.GOING;
                    actualMovementSpeed = movementSpeed;
                break;

                case WanderState.GOING:
                    HandleRotation();
                    //Debug.Log("[ERROR] Current State: GOING");
                    ProcessMovement();
                    //Debug.Log("[ERROR] Forward: " + gameObject.transform.GetForward());
                    gameObject.SetVelocity(gameObject.transform.GetForward() * actualMovementSpeed);
                break;


                case WanderState.CHASING:

                    //Debug.Log("[ERROR] Current State: CHASING");
                    vectorToPlayer = player.transform.globalPosition - gameObject.transform.globalPosition;
                    vectorToPlayer = Vector3.Normalize(vectorToPlayer);

                    xSpeed = vectorToPlayer.x;
                    ySpeed = vectorToPlayer.z;

                    RotateEnemy();
                    Debug.Log("[ERROR] Velocity: " + actualMovementSpeed);
                    gameObject.SetVelocity(gameObject.transform.GetForward() * actualMovementSpeed * 2);
                break;

                case WanderState.STOPED:
                    Debug.Log("[ERROR] Current State: STOPED");
                    ProcessStopped();
                break;

                case WanderState.HIT:

                    Proccescumdown();

                break;
            }


            if (player.transform.globalPosition.x - gameObject.transform.globalPosition.x < DetectionRadius && player.transform.globalPosition.z - gameObject.transform.globalPosition.z < DetectionRadius)
            {
                if (wanderState != WanderState.HIT)
                {
                    actualMovementSpeed = movementSpeed;
                    wanderState = WanderState.CHASING;
                }

            }

        }
        
       

    }


    private void Proccescumdown()
    {
        if(cumTimer > 0)
        {
            cumTimer -= Time.deltaTime;
            if(cumTimer<=0)
            {
                Debug.Log("[ERROR] Reached");
                wanderState = WanderState.REACHED;
            }
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
                actualMovementSpeed = 0;
                stopedTimer = stopedDuration;
                wanderState = WanderState.STOPED;

            }
        }
    }

    private void HandleRotation()
    {
        Vector3 aX = new Vector3(xSpeed, 0, ySpeed);
        aX = Vector3.Normalize(aX);

        //Debug.Log("[ERROR] Vector: " + aX);

        Quaternion targetRotation = Quaternion.identity;

        Vector3 aY = new Vector3(0,1,0);
        

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

    private void RotateEnemy()
    {
        Vector3 direction = player.transform.globalPosition - gameObject.transform.globalPosition;
        direction = direction.normalized;
        float angle = (float)Math.Atan2(direction.x, direction.z);

        //Debug.Log("Desired angle: " + (angle * Mathf.Rad2Deg).ToString());

        if (Math.Abs(angle * Mathf.Rad2Deg) < 1.0f)
            return;

        Quaternion dir = Quaternion.RotateAroundAxis(Vector3.up, angle);

        float rotationSpeed = Time.deltaTime * movementSpeed;
        //Debug.Log("CS: Rotation speed: " + rotationSpeed.ToString());
        //Debug.Log("CS: Time: " + Time.deltaTime);

        Quaternion desiredRotation = Quaternion.Slerp(gameObject.transform.localRotation, dir, rotationSpeed);

        gameObject.transform.localRotation = desiredRotation;

        //Debug.Log("[ERROR] rotation:  " + gameObject.transform.localRotation);
    }

    public void OnCollisionStay(GameObject other)
    {
        if (other.Name == "Player" && wanderState != WanderState.HIT)
        {

            Debug.Log("[ERROR] Name: " + other.Name);
            Debug.Log("[ERROR] HIT!!!");
            gameObject.SetVelocity(gameObject.transform.GetForward() * 0);
          
            gameObject.SetImpulse(gameObject.transform.GetForward() * -10);
            healthScript.TakeDmg(3);
            wanderState = WanderState.HIT;
            cumTimer = cumDuration;
        }
    }
}