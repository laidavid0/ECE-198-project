//variable declaration
const int trigPin = 9;
const int echoPin = 10;
//variable for function counting_rep
int rep_count{0};
double near_thershold{33.0};
double return_thershold{38.0};
bool state{false};
//variables for fucntions movement_range
double maximum{0};
double minimum{0};
double range{0};
bool has_measurement{false};
bool cycle_completed{false};
//variables for velocity
unsigned long start_time{0};
unsigned long end_time{0};
double previous_distance{0};
unsigned long previous_time{0};
bool has_previous{false};
//variables for noise_filers
double previous_filered{0};
bool has_filtered{false};
unsigned int alpha{0.2};


//function declaration
double abs(double x);
double distance_calculation(unsigned long duration);
int counting_rep(double distanceCm);
double movement_range(double distanceCm);
double velocity(double filter_distance);
double nosie_filer(double DistanceCm);

//absolute funciton
double abs(double x) {
  if (x > 0){
    return x;
  } else {
    return -x;
  }
}
//calculate distance
double distance_calculation(unsigned long duration){
  if (duration==0.0){
    return -1.0;
  } else{
    //the speed of sound is approximately 0.0343cm/us, as the sound travel back and forth so we divide the duration by 2 to get the distance in centimeter.
    return (duration*0.0343)/2.0;
  }
}
//counting repetition, and keep trakc of cycle repetition and elapsed time
int counting_rep(double distanceCm){
  cycle_completed = false;
  if (!state && distanceCm <= near_thershold){
    state=true;
    //we only record the start time when each repetition start
    start_time = millis();
  }else if (state && distanceCm >=return_thershold) {
    rep_count++;
    state = false;
    cycle_completed = true;
  }
  return rep_count;
}
//update maximum and minimum distance for each complete cycle and calculate movement range
double movement_range(double distanceCm){
  if (distanceCm < 0){
    return range;
  }
  if (state || cycle_completed){
    if(!has_measurement) {
      minimum = distanceCm;
      maximum = distanceCm;
      has_measurement = true;
    }
    if (distanceCm < minimum) {
      minimum = distanceCm;
    }
    if (distanceCm > maximum) {
      maximum = distanceCm;
    }
  }
  if (cycle_completed && has_measurement) {
    range = maximum - minimum;
    has_measurement = false;
  }
  return range;
}
//first compute signed velocity, and then use absoulte functiont to get speed
double velocity(double filter_distance){
  if (filter_distance < 0){
    return 0.0
  }
  unsigend long current_time = millis();
  if (!has_previous) {
    previous_distance = filter_distance;
    previous_time = current_time;
    has_previous = ture;
    return 0.0;
  }
  double displacement = filter_distance - previous distance;
  double time = current_time - previous_time;
  double speed= abs(displacement / time;
  return speed;

}

double nosie_filer(double DistanceCm) {
  if(DistanceCm < 0){
    return -1.0;
  }
  if(!has_filered) {
    previous_filtered = distance Cm;
    has_filtered = ture;
    return previous_filtered;
  }
  double filter_distance = alpha*distanceCm - (1-alpha)*previous_filtered;
  previous_filtered = filter_distance;
  return filter_distance;
}

void setup() {
 Serial.begin(115200);
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
}
void loop() {
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 unsigned long duration = pulseIn(echoPin, HIGH, 30000UL);
 double distanceCm = distance_calculation(duration);
 Serial.println(distanceCm);
 Serial.println(counting_rep(distanceCm));
 Serial.println(velocity(nosie_filer(distanceCm)));
 delay(100);
}
