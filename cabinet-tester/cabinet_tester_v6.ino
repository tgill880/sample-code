// measure effect of cabinet base mounting on cabinet vibration
// 50ms pulse
// caclulate rms of entire pulse
// average and sd of all pulses
// display average pulse for each aquisition
// full data set into out[]
// Thurman Gillespy

#include <Wire.h> // I2C communication
#include <SFE_MMA8452Q.h> // library for accelerometer

MMA8452Q accel; 

#define TONE_PIN  2
#define NOTE 100 // 100 Hz square wave
#define NOTE_DURATION 50 //  millisec
// 1250 microsec (1.25 millisec)
// for 800 Hz ODR (output data rate)
#define DELAY 1250 // microseconds
// 50ms pulse/1.25ms/sample = 40 samples
// continue to sample for another 20 samples
#define PULSE_SAMPLES 40
#define TOTAL_SAMPLES 60
#define OUT_SAMPLES 60
#define MOVING_AVG  3
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define NUM_AXES 3
#define SESSIONS 15
long  offset_x, offset_y, offset_z;
float temp = 0;
float value_x, value_y, value_z;
float sum_sq_x, sum_sq_y, sum_sq_z;
float sum_data_x, sum_data_y, sum_data_z;
float rms_x, rms_y, rms_z; // floats are 4 bytes
float mean_x, mean_y, mean_z;
float variance_x, variance_y, variance_z;
float sd_x, sd_y, sd_z;
float curTime = 0;
int i = 0, j = 0;
int session = 0;

int data[TOTAL_SAMPLES][NUM_AXES];
int out[TOTAL_SAMPLES][NUM_AXES];
float rms[SESSIONS][NUM_AXES];
String location = "BBP1";
String method = "no signal, spikes";

void setup() {
  accel.init(); // 800 Hz, +/- 2G
  Serial.begin(19200);
  Serial.println();
  Serial.println("Speaker testing v6");
  Serial.println();

  delay(200);
}

void loop() {
  if (accel.available())
  {
    // repeat number of SESSIONS
    for (session = 0; session < SESSIONS; session++)
    {
      // first get offset values for each axis
      // will include pos and neg values
      offset_x = 0; offset_y = 0; offset_z = 0;
      for (i = 0; i < 100; i++)
      {
        curTime = micros();
        accel.read();
        offset_x += accel.x;
        offset_y += accel.y;
        offset_z += accel.z;
        // one read every 1.25 millisec
        while (micros() - curTime <= DELAY)
          ;
      }
      offset_x /= i;
      offset_y /= i;
      offset_z /= i;

      // play a 100 Hz tone on pin2 for 50 millisec (40 samples)
      // and continue to measure for TOTAL_SAMPLES
      tone(TONE_PIN, NOTE, NOTE_DURATION); // non-blocking
      for (i = 0; i < TOTAL_SAMPLES; i++)
      {
        curTime = micros();
        accel.read();
        data[i][X_AXIS] = accel.x - offset_x;
        data[i][Y_AXIS] = accel.y - offset_y;
        data[i][Z_AXIS] = accel.z - offset_z;
        // one read every 1.25 millisec
        while (micros() - curTime <= DELAY)
          ;
      } // end while

      // calculate rms of the entire pulse
      // put into rms[SESSIONS][NUM_AXES]
      sum_sq_x = 0; sum_sq_y = 0; sum_sq_z = 0;
      for (i = 0; i < PULSE_SAMPLES; i++)
      {
        sum_sq_x += (float)data[i][X_AXIS] * (float)data[i][X_AXIS];
        sum_sq_y += (float)data[i][Y_AXIS] * (float)data[i][Y_AXIS];
        sum_sq_z += (float)data[i][Z_AXIS] * (float)data[i][Z_AXIS];
      }
      // calc rms
      rms_x = sqrt(sum_sq_x / (float)PULSE_SAMPLES);
      rms_y = sqrt(sum_sq_y / (float)PULSE_SAMPLES);
      rms_z = sqrt(sum_sq_z / (float)PULSE_SAMPLES);
      
      // place into rms[]
      rms[session][X_AXIS] = rms_x;
      rms[session][Y_AXIS] = rms_y;
      rms[session][Z_AXIS] = rms_z;

      // moving 3-point rms of data, place into out[]
      for (i = 0; i < TOTAL_SAMPLES - MOVING_AVG; i++)
      {
        sum_sq_x = 0; sum_sq_y = 0; sum_sq_z = 0;
        for (j = 0; j < MOVING_AVG; j++)
        {
          sum_sq_x += (long)data[i + j][X_AXIS] * (long)data[i + j][X_AXIS];
          sum_sq_y += (long)data[i + j][Y_AXIS] * (long)data[i + j][Y_AXIS];
          sum_sq_z += (long)data[i + j][Z_AXIS] * (long)data[i + j][Z_AXIS];
        }
        rms_x = sqrt(sum_sq_x / (float)MOVING_AVG);
        rms_y = sqrt(sum_sq_y / (float)MOVING_AVG);
        rms_z = sqrt(sum_sq_z / (float)MOVING_AVG);

        
        out[i][X_AXIS] += rms_x;
        out[i][Y_AXIS] += rms_y;
        out[i][Z_AXIS] += rms_z;
      }
      delay(2000); // wait 2 seconds between sessions
    } // end for session

    // average the data values in out[] by number of SESSIONS
    for (i = 0; i < OUT_SAMPLES - MOVING_AVG; i++)
    {
      out[i][X_AXIS] /= SESSIONS;
      out[i][Y_AXIS] /= SESSIONS;
      out[i][Z_AXIS] /= SESSIONS;
    }

    // calc mean, sd of rms data in rms[]
    sum_data_x = 0; sum_data_y = 0; sum_data_z = 0;
    sum_sq_x = 0; sum_sq_y = 0; sum_sq_z = 0;
    for (i = 0; i < SESSIONS; i++)
    {
      value_x = rms[i][X_AXIS];
      value_y = rms[i][Y_AXIS];
      value_z = rms[i][Z_AXIS];

      sum_data_x += value_x;
      sum_data_y += value_y;
      sum_data_z += value_z;

      sum_sq_x += value_x * value_x;
      sum_sq_y += value_y * value_y;
      sum_sq_z += value_z * value_z;
    }
    mean_x = sum_data_x/(float)SESSIONS;
    mean_y = sum_data_y/(float)SESSIONS;
    mean_z = sum_data_z/(float)SESSIONS;

    variance_x = (sum_sq_x - ((sum_data_x * sum_data_x) /
                 (double)SESSIONS) ) /(double)(SESSIONS - 1);
    variance_y = (sum_sq_y - ((sum_data_y * sum_data_y) /
                 (double)SESSIONS) ) /(double)(SESSIONS - 1);
    variance_z = (sum_sq_z - ((sum_data_z * sum_data_z) /
                 (double)SESSIONS) ) /(double)(SESSIONS - 1);
    sd_x = sqrt(variance_x);
    sd_y = sqrt(variance_y);
    sd_z = sqrt(variance_z);
    
    // display out data in g
    Serial.println("X-axis\tY-axis\tZ-axis");
    for (i = 0; i < TOTAL_SAMPLES - MOVING_AVG; i++)
    {
      temp = (float)out[i][X_AXIS] / 1024.0;
      Serial.print(temp, 4); // X-axis
      Serial.print("\t");
      temp = (float)out[i][Y_AXIS] / 1024.0;
      Serial.print(temp, 4); // Y-axis
      Serial.print("\t");
      temp = (float)out[i][Z_AXIS] / 1024.0;
      Serial.println(temp, 4); // Z-axis
    }


    // info
    Serial.println(); Serial.println();
    Serial.println("Speaker testing software v6");
    Serial.print("test location "); Serial.println(location);
    Serial.print("N = "); Serial.println(SESSIONS);
    Serial.print("method: "); Serial.println(method);
    Serial.println();
    
    // print stats
    Serial.print("X-axis: "); Serial.print("mean = "); Serial.print(mean_x/1024.0, 4); 
    Serial.print(", sd = "); Serial.println(sd_x/1024.0, 4);

    Serial.print("Y-axis: "); Serial.print("mean = "); Serial.print(mean_y/1024.0, 4); 
    Serial.print(", sd = "); Serial.println(sd_y/1024.0, 4);

    Serial.print("Z-axis: "); Serial.print("mean = "); Serial.print(mean_z/1024.0, 4); 
    Serial.print(", sd = "); Serial.println(sd_z/1024.0, 4);

    Serial.println();
    
  } // end if accel

  Serial.println("Bye");
  Serial.flush();

  exit(EXIT_SUCCESS);
}
