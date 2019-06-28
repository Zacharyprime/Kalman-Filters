/*
Explaination of Kalman Filters
By: Zachary Sasser
 
A kalman filter is basically a fast way to find a real value for a sensor.
(A fast way to find averages in a way but don't mistake it as a way to find a mathematical average)
 
We could just take a ton of data and take the average and remove outliers and it probably would give you the true value you want.
But taking the average method usually requires a lot of data to get accurate and
on timing intervals like acceleration that means inaccuracy and timing issues.
 
A kalman filter can converge to a value with much less data than a simple average
and when combined with things like a low pass filter you can get pretty good accuracy
on pretty inaccurate hardware.
 
*/

//Use pathagorean theorum to get the magnetude of acceleration.
//If the values are negative the value will come out negative (because CS != math) and so use abs(); (absolute value)
int getAccel() {
  return abs(sqrt(pow(accel_x(), 2) + pow(accel_y(), 2) + pow(accel_z(), 2)));
}

double KG; //The kalman gain (a coefficient explained later I'm just defining it here)
double change; //You normally don't use this I'm seperating the variables to explain things easier

int main() {

  //Get an inital guess at what we think the number probably is.
  //You can actually set this value to pretty much anything random because the kalman filter will narrow down the value very fast anyway.
  double estimation = getAccel();

  //Print our initial estimate so its tangeble for us to understand
  //NOTE IF YOU DON'T DO %f you'll get giant values because we are working with doubles not ints
  printf("Initial Estimate: %f \n", estimation);

  //This is what the error in each of our measurements probably is.
  //If this value is large it will have slower performance and the error takes longer to reduce
  //However if the value is too low you might get completely wrong values or you won't end up with as accurate of values
  //(because the formula thinks that you're measurements are more accurate than they are)
  double measurementError = 50;

  //This is how much error we think is in the current estimate (in this case our initial estimate)
  //It can be used as a general measurement of how much error our estimation is so far
  //This value can also be pretty much anything but if it's too low you might get bad values
  double estimationError = 50;

  double targetError = 10;
  //how much error we'd like to have in our estimate after
  //(note you might want to do iterations instead of a error)

  while (estimationError < targetError) {

    //STEP ONE: Kalman Gain
    //The kalman gain is a measurement of how close to the true value we probably are.
    //In other words it takes into account how much error we have compared to the error in the measurement.
    //We will use this later to make sure that new values don't change the current one as much.
    KG = estimationError / (measurementError + estimationError);

    //STEP TWO: Update Estimation
    //This is how much our estimate needs to change taking into account the new data (getAccel())
    //Note as our program goes on the value of KG goes down
    //If KG is small (close to zero) this change value will be almost nothing because new data shouldn't change our estimation by much
    //At the beginning KG is larger (close to 1) and so the new data has a lot more pull on what our next estimation should be.

    //Basically if we are already pretty certain about our guess, we don't want noise or a measurement to change our guess
    //to a completely different value.
    change = KG * (getAccel() - estimation);

    //We add this change to our estimation to update what we think the value should be.
    //NOTE: Normally you don't split up the variables like this you'd just add them in one go to preserve speed and memory.
    //I split it up here to help explain that it is a CHANGE to the estimation when new data is added.
    estimation = estimation + change;

    //STEP THREE: Update Reported Error
    //Now that our estimation should be more accurate we need to update our reported error as such.
    //If KG is close to 0 then we should be at a fairly low error or high accuracy so the error wont change as much from this new data
    //If KG is close to 1 then the error should change by more because the new values are updating the value a lot more
    estimationError = (1 - KG) * estimationError;

  }

}
