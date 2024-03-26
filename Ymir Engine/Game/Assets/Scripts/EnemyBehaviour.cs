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
    Reached,
    Going,
    Stoped
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

    private bool start = true;

    private float movementSpeed = 2f, movementAux = 2f;

    public float life = 100f;

    private float wanderRadius = 10f;

    public float xSpeed = 0, ySpeed = 0;

    private int counter1 = 0;

    //public float wanderTimer = 5f;

    private GameObject playerObject;

    public bool PlayerDetected = false;

    public float DetectionRadius = 15f;

    //private EnemyState state = EnemyState.Idle;

    private WanderState wanderState = WanderState.Reached;

    RandomPointGenerator pointGenerator;

    private Vector3 vectorToPlayer = null;

    public void Update()
	{
        thisReference = gameObject;

        if (start)
        {
            pointGenerator = new RandomPointGenerator();
            playerObject = InternalCalls.GetGameObjectByName("Player");
            start = false;
        }

        if (PlayerDetected == false)
        {

            if (wanderState == WanderState.Reached)
            {
                (xSpeed, ySpeed) = pointGenerator.GetRandomPointInRadius(wanderRadius);

                wanderState = WanderState.Going;
                //state = EnemyState.Moving;
            }

            if (wanderState == WanderState.Going)
            {
                HandleRotation();

                counter1++;

                gameObject.SetVelocity(gameObject.transform.GetForward() * -movementSpeed);

                //Set movement speed negative cuz the facehugger is facing backwards
                //if (counter1 > 80 && counter1 < 320)
                //{
                //    movementSpeed = 0;
                //}
                if (counter1 > 320)
                {
                    //movementSpeed += movementAux;
                    //Debug.Log("[ERROR] Speed " + movementSpeed);
                    counter1 -= 320;
                    wanderState = WanderState.Reached;
                }

                if (playerObject.transform.globalPosition.x - gameObject.transform.globalPosition.x < DetectionRadius && playerObject.transform.globalPosition.y - gameObject.transform.globalPosition.y < DetectionRadius)
                {
                    PlayerDetected = true;
                }

                //if (counter1 > 80)
                //{
                //    movementSpeed = 0;
                //    counter2++;
                //    if (counter2 > 320)
                //    {
                //        counter1 = 0;
                //        counter2 = 0;
                //        wanderState = WanderState.Reached;
                //    }
                //}
            }
        }
        else
        {
            vectorToPlayer = playerObject.transform.globalPosition - gameObject.transform.globalPosition;
            vectorToPlayer = Vector3.Normalize(vectorToPlayer);
            xSpeed = -vectorToPlayer.x;
            ySpeed = -vectorToPlayer.z;  

            HandleRotation();

            gameObject.SetVelocity(gameObject.transform.GetForward() * -movementSpeed);

        }
    }

    private void HandleRotation()
    {
        Vector3 aX = new Vector3(xSpeed, 0, ySpeed);
        aX = Vector3.Normalize(aX);

        Quaternion targetRotation = Quaternion.identity;

        Vector3 aY = gameObject.transform.GetUp();

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

            angle -= Mathf.PI / 4f;

            targetRotation = Quaternion.AngleAxis(angle * Mathf.Rad2Deg, Vector3.up);
        }
        gameObject.SetRotation(targetRotation);

    }
}