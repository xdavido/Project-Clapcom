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

public enum WanderState
{
    REACHED,
    GOING,
    CHASING,
    ATTACK,
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

public class FaceHuggerBaseScript : YmirComponent
{
    public GameObject thisReference = null;


    public float movementSpeed = 20f;
    private float actualMovementSpeed;

    public float life = 100f;

    //This may change depending on enemy rarity
    public float armor = 0;

    //0 = Common, 1 = Rare, 2 = Elite
    public int rarity = 0;

    private float wanderRange = 10f;

    public float xSpeed = 0, ySpeed = 0;


    //public float wanderTimer = 5f;

    private GameObject player;
    protected PathFinding agent;

    public bool PlayerDetected = false;

    public float DetectionRadius = 60f;
    private float AttackDistance = 20f;

    //private EnemyState state = EnemyState.Idle;

    private WanderState wanderState;

    RandomPointGenerator pointGenerator;
    private Health healthScript;

   

    private float wanderTimer;
    public float wanderDuration = 5f;

    private float stopedTimer;
    public float stopedDuration = 1f;

    private float cumTimer;
    public float cumDuration = 2f;

    private float cumTimer2;
    public float cumDuration2 = 5f;

    public float attackTimer;
    private float attackDuration = 1.5f;
    public bool attackSensor = false;

    public void Start()
    {
        pointGenerator = new RandomPointGenerator();
        wanderState = WanderState.REACHED;
        wanderDuration = 5f;
        wanderTimer = wanderDuration;
        player = InternalCalls.GetGameObjectByName("Player");
        healthScript = player.GetComponent<Health>();
        agent = gameObject.GetComponent<PathFinding>();
        agent.stoppingDistance = 1f;
        agent.speed = 10f;
        movementSpeed = 20f;
        stopedDuration = 1f;
        DetectionRadius = 60f;
        wanderRange = 100f;
        cumDuration = 2f;
        cumDuration2 = 5f;

        attackTimer = attackDuration;
        agent.stoppingDistance = 1f;

        cumTimer = cumDuration2;

        //Enemy rarity stats
        if (rarity == 1)
        {
            life = 350;
            armor = 0.1f;
            movementSpeed = 21.5f;
        }
        else if (rarity == 2)
        {
            life = 600;
            armor = 0.2f;
            movementSpeed = 23f;

        }

        //gameObject.SetPosition(new Vector3(-181,4,230));

    }

    public void Update()
    {
   
        cumTimer2 -= Time.deltaTime;
        if(cumTimer2 <= 0)
        {
            switch (wanderState)
            {
                case WanderState.REACHED:
                    agent.CalculateRandomPath(gameObject.transform.globalPosition, wanderRange);
                    wanderTimer = wanderDuration;
                    Debug.Log("[ERROR] Current State: REACHED");
                    wanderState = WanderState.GOING;
                    break;

                case WanderState.GOING:
                    agent.speed = 30f;

                    LookAt(agent.GetDestination());
                    LookAt(agent.GetDestination());
                    MoveToCalculatedPos(agent.speed);
                    Debug.Log("[ERROR] Current State: GOING");

                    //if(Mathf.Distance(gameObject.transform.globalPosition,agent.GetPointAt(agent.GetPathSize())) < 1)
                    //{
                    //    wanderState = WanderState.REACHED; break;
                    //}
                    break;


                case WanderState.CHASING:

                    //Debug.Log("[ERROR] Current State: CHASING");
                    agent.CalculatePath(gameObject.transform.globalPosition, player.transform.globalPosition);

                    MoveToCalculatedPos(agent.speed);
                    
                    Debug.Log("[ERROR] Current State: CHASING");
                    break;

                case WanderState.STOPED:
                    //Debug.Log("[ERROR] Current State: STOPED");
                    ProcessStopped();
                break;

                case WanderState.HIT:

                    Proccescumdown();

                break;

                case WanderState.ATTACK:

                    Attack();
                break;
            }

            //Check if player is alive before chasing
            if (wanderState != WanderState.ATTACK && healthScript.GetCurrentHealth() > 0)
            {

                if (CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, DetectionRadius))
                {

                    if (wanderState != WanderState.HIT)
                    {
                        actualMovementSpeed = movementSpeed;
                        wanderState = WanderState.CHASING;
                    }

                    //Attack if in range
                    if (CheckDistance(player.transform.globalPosition, gameObject.transform.globalPosition, AttackDistance))
                    {

                        if (wanderState == WanderState.CHASING && wanderState != WanderState.ATTACK)
                        {
                            //Debug.Log("[ERROR] ATTACKING");
                            attackTimer = attackDuration;
                            gameObject.SetVelocity(gameObject.transform.GetForward() * 0);
                            wanderState = WanderState.ATTACK;
                        }
                    }

                }
            }


        }



    }

    public bool CheckDistance(Vector3 first, Vector3 second, float checkRadius)
    {
        float deltaX = Math.Abs(first.x - second.x);
        float deltaY = Math.Abs(first.y - second.y);
        float deltaZ = Math.Abs(first.z - second.z);

        return deltaX <= checkRadius && deltaY <= checkRadius && deltaZ <= checkRadius;
    }

    private void Proccescumdown()
    {
        if(cumTimer > 0)
        {
            cumTimer -= Time.deltaTime;
            if(cumTimer<=0)
            {
                //Debug.Log("[ERROR] Reached");
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

                //Debug.Log("[ERROR] AAAA");
                actualMovementSpeed = 0;
                stopedTimer = stopedDuration;
                wanderState = WanderState.STOPED;

            }
        }
    }

    public WanderState GetState()
    {
        return wanderState;
    }
    private void Attack()
    {
        if (attackTimer > 0)
        {
            attackTimer -= Time.deltaTime;

            if (attackTimer <= 0)
            {
                ////IF HIT, Do damage
                //healthScript.TakeDmg(3);
                //Debug.Log("[ERROR] DID DAMAGE");
                attackSensor = true;
                attackTimer = attackDuration;
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

    public void LookAt(Vector3 pointToLook)
    {
        Vector3 direction = pointToLook - gameObject.transform.globalPosition;
        direction = direction.normalized;
        float angle = (float)Math.Atan2(direction.x, direction.z);

        //Debug.Log("Desired angle: " + (angle * Mathf.Rad2Deg).ToString());

        if (Math.Abs(angle * Mathf.Rad2Deg) < 1.0f)
            return;

        Quaternion dir = Quaternion.RotateAroundAxis(Vector3.up, angle);

        float rotationSpeed = Time.deltaTime * agent.angularSpeed * 100;


        Quaternion desiredRotation = Quaternion.Slerp(gameObject.transform.localRotation, dir, rotationSpeed);

        gameObject.transform.localRotation = desiredRotation;

    }

    public void MoveToCalculatedPos(float speed)
    {
        Vector3 pos = gameObject.transform.globalPosition;
        Vector3 destination = agent.GetDestination();
        Vector3 direction = destination - pos;

        gameObject.SetVelocity(direction.normalized * 5f);
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
        //if (other.Name == "Player" && wanderState != WanderState.HIT)
        //{

        //    Debug.Log("[ERROR] Name: " + other.Name);
        //    Debug.Log("[ERROR] HIT!!!");
        //    gameObject.SetVelocity(gameObject.transform.GetForward() * 0);
          
        //    gameObject.SetImpulse(gameObject.transform.GetForward() * -10);
        //    healthScript.TakeDmg(3);
        //    wanderState = WanderState.HIT;
        //    cumTimer = cumDuration;
        //}
    }
}