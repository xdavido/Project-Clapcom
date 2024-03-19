using System;
using System.Collections.Generic;
using YmirEngine;

public class EnemyBehaviourTest : YmirComponent
{

    public GameObject thisReference = null;

    public Vector3 currentPos = gameObject.transform.localPosition;

    public float movementSpeed = 0.1f;

    public float life = 100f;

    public float wanderRadius = 10f;

    public float xToGo = 0, yToGo = 0;

    public enum EnemyState
    {
        Idle,
        Moving,
        Attacking,
        Dead
    }
    
    public enum WanderState
    {
        Reached,
        Going,
        Stoped
    }

    public EnemyState state = EnemyState.Idle;

    public WanderState wanderState = WanderState.Reached;

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

    RandomPointGenerator pointGenerator = new RandomPointGenerator();

    public void Update()
    {
        thisReference = gameObject;

        if (wanderState == WanderState.Reached)
        {
            (xToGo, yToGo) = pointGenerator.GetRandomPointInRadius(wanderRadius);
            currentPos += new Vector3(xToGo, 0, 0);
            currentPos += new Vector3(0, 0, yToGo);
            wanderState = WanderState.Going;
        }

        if (wanderState == WanderState.Stoped)
        {
            //Doing both in the same line doesn't work right
            gameObject.transform.localPosition += new Vector3(xToGo, 0, 0) * movementSpeed * Time.deltaTime;
            gameObject.transform.localPosition += new Vector3(0, 0, yToGo) * movementSpeed * Time.deltaTime;

            if (gameObject.transform.localPosition.x - currentPos.x < 1 || gameObject.transform.localPosition.y - currentPos.y < 1 || gameObject.transform.localPosition.z - currentPos.z < 1)
            {
                wanderState = WanderState.Reached;
            }

        }

    }
}