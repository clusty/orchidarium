/*typedef struct {
  double q; //process noise covariance
  double r; //measurement noise covariance
  double x; //value
  double p; //estimation error covariance
  double k; //kalman gain
} kalman_state;

kalman_state
kalman_init(double q, double r, double p, double intial_value)
{
  kalman_state result;
  result.q = q;
  result.r = r;
  result.p = p;
  result.x = intial_value;

  return result;
}

void
kalman_update(kalman_state* state, double measurement)
{
  //prediction update
  //omit x = x
  state->p = state->p + state->q;

  //measurement update
  state->k = state->p / (state->p + state->r);
  state->x = state->x + state->k * (measurement - state->x);
  state->p = (1 - state->k) * state->p;
}
*/

// This #include statement was automatically added by the Particle IDE.
#include "ThingSpeak/ThingSpeak.h"

// This #include statement was automatically added by the Particle IDE.
#include "SHT30/SHT30.h"

unsigned long myChannelNumber = 143245;
const char * myWriteAPIKey = "YKLVL7RWFDXY0MCI";

SHT30 sensorCold, sensorHot;
TCPClient client;
double humidity;
double temperature;

void setup() {
    sensorCold.setAddress(0);
    sensorHot.setAddress(1);
    Particle.variable("HLow", humidity);
    Particle.variable("TLow", temperature);
    Particle.variable("HHigh", humidity);
    Particle.variable("THigh", temperature);
    ThingSpeak.begin(client);
}

void loop() {
    if(sensorCold.update()){
        humidity = sensorCold.humidity;
        temperature = sensorCold.temperature;
        Particle.publish("HLow", String(humidity));
        Particle.publish("TLow", String(temperature));
        ThingSpeak.setField(1, static_cast<float>(temperature));
        ThingSpeak.setField(2, static_cast<float>(humidity));
    }
    else
    {
         Particle.publish("TLow", String(-1));
    }

    if(sensorHot.update()){
        humidity = sensorHot.humidity;
        temperature = sensorHot.temperature;
        Particle.publish("HHigh", String(humidity));
        Particle.publish("THigh", String(temperature));
        ThingSpeak.setField(3, static_cast<float>(temperature));
        ThingSpeak.setField(4, static_cast<float>(humidity));
    }
    else
    {
         Particle.publish("HLow", String(-1));
    }

    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    delay(10000);
}
