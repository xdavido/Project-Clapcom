using System;
using System.Collections.Generic;
using System.IO;
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

    public float movementSpeed = 2f;

    public float life = 100f;

    public float wanderRadius = 10f;

    public float xSpeed = 0, ySpeed = 0;

    private int counter1 = 0, counter2 = 0;

    private double angle = 0.0f;

    public float rotationSpeed = 0.5f;

    private bool rotate = false;

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
            rotate = true;
            //state = EnemyState.Moving;
        }

        if (wanderState == WanderState.Going)
        {
            HandleRotation();

            //Set movement speed negative cuz the facehugger is facing backwards
            gameObject.SetVelocity(gameObject.transform.GetForward() * -movementSpeed);

            counter1++;

            if (counter1 > 500)
            {
                wanderState = WanderState.Reached;
                counter1 = 0;
            }
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