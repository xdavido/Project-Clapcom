using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using YmirEngine;

public class UI_Animation : YmirComponent
{
    public bool loop = false;
    public bool pingpong = false;
    public bool backwards = false;

    public float speed = 1f;
    public int loopCount = 0;
    public int duration = 1;

    public GameObject image = null;

    public Vector2 rowXcolumn, currentFrame;

    private float _timer = 0;

    public void Start()
    {
        Debug.Log("HelloWorld");

        image = InternalCalls.GetGameObjectByName("Image");
        rowXcolumn = UI.GetImageRowsAndColumns(image);
    }

    public void Update()
    {
        //if (loop || loopCount == 0 && (_timer += Time.deltaTime) >= speed)
        //{
        //    _timer = 0;

        //    if (!backwards)
        //    {
        //        if (currentFrame.x < rowXcolumn.x) { ++currentFrame.x; }
        //        if (currentFrame.y < rowXcolumn.y) { ++currentFrame.y; }

        //        UI.SetImageCurrentFrame(image, (int)currentFrame.x, (int)currentFrame.y);
        //    }
        //}

        // Check if it's time to update the frame
        if (!HasFinished() && (_timer += Time.deltaTime) >= speed)
        {
            _timer = 0;
            loopCount++;

            // Increment or decrement the current frame based on direction
            if (!backwards)
            {
                currentFrame.x++;

                if (currentFrame.x >= rowXcolumn.x)
                {
                    currentFrame.x = 0;
                    currentFrame.y++;

                    // Check if ping-pong animation is enabled and we reached the last row
                    if (pingpong && currentFrame.y >= rowXcolumn.y)
                    {
                        backwards = true;
                        currentFrame.y = Mathf.Max(rowXcolumn.y - 2, 0); // Start from the second-to-last row
                    }

                    // If looping and not ping-ponging, reset to the first row
                    else if (loop && currentFrame.y >= rowXcolumn.y)
                    {
                        currentFrame.y = 0;
                    }
                }
            }
            else
            {
                currentFrame.x--;
                if (currentFrame.x < 0)
                {
                    currentFrame.x = (int)rowXcolumn.x - 1;
                    currentFrame.y--;

                    // Check if ping-pong animation is enabled and we reached the first row
                    if (pingpong && currentFrame.y < 0)
                    {
                        backwards = false;
                        currentFrame.y = 1; // Start from the second row
                    }

                    // If looping and not ping-ponging, reset to the last row
                    else if (loop && currentFrame.y < 0)
                    {
                        currentFrame.y = (int)rowXcolumn.y - 1;
                    }
                }
            }

            // Update the current frame of the image
            UI.SetImageCurrentFrame(image, (int)currentFrame.x, (int)currentFrame.y);
        }

        return;
    }

    bool HasFinished()
    {
        return !loop && !pingpong && loopCount > 0;
    }

    void Reset()
    {
        loopCount = 0;
    }
}