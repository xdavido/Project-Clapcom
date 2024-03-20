using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

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

    public float movementSpeed = 0.1f;

    public float life = 100f;

    public float wanderRadius = 10f;

    public float xSpeed = 0, ySpeed = 0;

    private int counter = 0;

    //private EnemyState state = EnemyState.Idle;

    private WanderState wanderState = WanderState.Reached;

    RandomPointGenerator pointGenerator;

    public void Update()
	{
        thisReference = gameObject;

        if (start)
        {
            pointGenerator = new RandomPointGenerator();
            start = false;
        }

        if (wanderState == WanderState.Reached)
        {
            (xSpeed, ySpeed) = pointGenerator.GetRandomPointInRadius(wanderRadius);

            wanderState = WanderState.Going;
            //state = EnemyState.Moving;
        }

        if (wanderState == WanderState.Going)
        {
            //Doing both in the same line doesn't work right
            gameObject.transform.localPosition += new Vector3(xSpeed, 0, 0) * movementSpeed * Time.deltaTime;
            gameObject.transform.localPosition += new Vector3(0, 0, ySpeed) * movementSpeed * Time.deltaTime;

            counter++;

            if (counter > 500)
            {
                wanderState = WanderState.Reached;
                counter = 0;
            }

        }
    }
}