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

    public float movementSpeed = 0.2f;

    public float life = 100f;

    public float wanderRadius = 10f;

    public float xSpeed = 0, ySpeed = 0;

    private int counter1 = 0;
    
    //private int counter2 = 0;

    //private double angle = 0.0f;

    //public float rotationSpeed = 0.1f;

    //private bool rotate = false;

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
            //rotate = true;
            //state = EnemyState.Moving;
        }

        if (wanderState == WanderState.Going)
        {
            gameObject.SetVelocity(new Vector3(xSpeed, -5, ySpeed) * movementSpeed);

            counter1++;

            if (counter1 > 500)
            {
                wanderState = WanderState.Reached;
                counter1 = 0;
            }

            ////Rotation
            ////Se viene hardcodeada

            //if (rotate == true)
            //{
            //    if (counter2 == 0)
            //    {
            //        Vector3 X = new Vector3(xSpeed, 0, -ySpeed - 1);
            //        Vector3 Y = new Vector3(0, 0, 1);
            //        X = Vector3.Normalize(X);

            //        if (X.x >= 0)
            //        {
            //            angle = Math.Acos(Vector3.Dot(X, Y) - 1);
            //        }
            //        else if (X.x < 0)
            //        {
            //            angle = -Math.Acos(Vector3.Dot(X, Y) - 1);
            //        }

            //        angle = angle / 20;
            //        counter2++;

            //    }

            //    if (counter2 > 0)
            //    {
            //        counter2++;
            //        angle = angle + (angle / counter2);
            //        gameObject.transform.localRotation = Quaternion.RotateAroundAxis(Vector3.up, (float)-angle);
            //        if (counter2 > 20)
            //        {
            //            counter2 = 0;
            //            rotate = false;
            //            Vector3 X = new Vector3(xSpeed, 0, -ySpeed - 1);
            //            Vector3 Y = new Vector3(0, 0, 1);
            //            X = Vector3.Normalize(X);

            //            if (X.x >= 0)
            //            {
            //                angle = Math.Acos(Vector3.Dot(X, Y) - 1);
            //            }
            //            else if (X.x < 0)
            //            {
            //                angle = -Math.Acos(Vector3.Dot(X, Y) - 1);
            //            }
            //            gameObject.transform.localRotation = Quaternion.RotateAroundAxis(Vector3.up, (float)-angle);

            //        }

            //    }
            //}

        }
    }
}