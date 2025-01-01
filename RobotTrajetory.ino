#define ROBOT_DIAMETER 14.3
#define WHEEL_RADIUS 4.2
#define CELL_DISTANCE 40
#define RPS 2.3
#define V 255
//H Bridge Control Class
class DCMotor {
  int spd = 255, pin1, pin2;

  public:

    void Pinout(int in1, int in2){
        
      pin1 = in1;
      pin2 = in2;
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
      
    }

    void Speed(int in1){

      spd = in1;
      
    }

    void Forward(){

      analogWrite(pin1, spd);
      digitalWrite(pin2, LOW);
      
    }

    void Backward(){
      digitalWrite(pin1, LOW);
      analogWrite(pin2, spd);  
      
    }

    void Stop(){

      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
    
    }
    
};

//Prototypes
void move_robot(float distance);
void turn_robot(int o_orientation, int angle);
void turnLeft();
void turnRight();
int findOpenParenteses(String str, int startIdx);
int findCloseParenteses(String str, int startIdx);
int findComma(String str);
void runTrajectory(String path_data, String initial_position, int d_teta);
void move_and_turn(int orientation, int distance);

//Variables
int idx  = 0;
int startIdx = 0;
int endIdx = 0;
int len = 0;
int count = 0;
int o_orientation = 0;
String tupla;
String pos_o;
DCMotor Motor1, Motor2;

void setup() {
  Serial.begin(9600);
  Motor1.Pinout(5,6);
  Motor2.Pinout(10,11);
  Motor1.Speed(V);
  Motor2.Speed(V);
  String path_data = "[(6, 1), (7, 1), (8, 1), (9, 1), (10, 1), (11, 1), (12, 1), (13, 1), (13, 2), (13, 3), (13, 4), (13, 5), (13, 6), (13, 7), (13, 8), (13, 9), (13, 10), (13, 11), (13, 12), (13, 13), (13, 14), (13, 15), (13, 16), (14, 16), (15, 16), (16, 16)]";
  runTrajectory(path_data, "(6, 0)", 0);
}

void loop() {
}

//Simplified H-bridge Control Functions Block
void move_robot(float distance) {
  float rotations_targent = distance/(2*PI*WHEEL_RADIUS);
  float time = (rotations_targent/RPS) * 1000;
  Motor1.Forward();
  Motor2.Forward();
  delay(time);
  Motor1.Stop();
  Motor2.Stop();
}

void turnRight(){
  Motor1.Forward();
  Motor2.Backward();
}

void turnLeft(){
  Motor1.Backward();
  Motor2.Forward();
}

void turn_robot(int o_orientation, int angle){
  float t_rotations = ((angle*RPS)/720)*1000;
  switch(o_orientation){
    case 90:
      turnRight();
      break;
    case 180:
      turnRight();
      break;
    case 270:
      turnLeft();
      break;
    default:
      break;
  }
  delay(t_rotations);
  Motor1.Stop();
  Motor2.Stop();
}

void move_and_turn(int orientation, int distance, int angle){
  turn_robot(orientation,angle);
  delay(1000);
  move_robot(distance);
  delay(1000);
}


//Robot trajectory control block based on approximate cell decomposition
int findOpenParenteses(String str, int startIdx){
  for(int i = startIdx; i < str.length() ; i++){
    if(str[i] == '('){
      return i;
    }
  }
  return -1;
}

int findCloseParenteses(String str, int startIdx){
  for(int i = startIdx; i < str.length(); i++){
    if(str[i] == ')'){
      return i;
    }
  }
  return -1;
}

int findComma(String str){
  for(int i = 0; i < str.length(); i++){
    if(str[i] == ','){
      return i;
    }
  }
  return -1;
}

void runTrajectory(String path_data, String initial_position, int d_teta){
  String current_position = initial_position;
  int current_orientation = d_teta;
  while(true){

    startIdx = findOpenParenteses(path_data, idx);
    if(startIdx == -1) break;

    endIdx = findCloseParenteses(path_data, startIdx);
    if(endIdx == -1) break;
    
    tupla = path_data.substring(startIdx, endIdx + 1);
    
    // Extracting coordinates of current position and next position
    int CommaPos = findComma(current_position);
    int YOaux = (current_position.substring(1, CommaPos - 1)).toInt();
    int XOaux = (current_position.substring(CommaPos + 1, current_position.length() - 1)).toInt();
    
    CommaPos = findComma(tupla);
    int Yaux = (tupla.substring(1, CommaPos)).toInt();
    int Xaux = (tupla.substring(CommaPos + 1, tupla.length() - 1)).toInt();
    Serial.print(Yaux);
    Serial.print("(y)-(x)");
    Serial.println(Xaux);

    //Calculating changes in X and Y
    int DeltaY = Yaux - YOaux;
    int DeltaX = Xaux - XOaux;

    if(DeltaX > 0){
      if(current_orientation == 0){
        move_robot(CELL_DISTANCE);
      }else if(current_orientation == 90){
        move_and_turn(90, CELL_DISTANCE, 90);
        current_orientation = 0;
      }else if(current_orientation == 180){
        move_and_turn(180, CELL_DISTANCE, 180);
        current_orientation = 0;
      }else if(current_orientation == 270){
        move_and_turn(270, CELL_DISTANCE, 90);
        current_orientation = 0;
      }
    }else if(DeltaX < 0){
      if(current_orientation == 180){
        move_robot(CELL_DISTANCE);
      }else if(current_orientation == 0){
        move_and_turn(180, CELL_DISTANCE, 90);
        current_orientation = 180;
      }else if(current_orientation == 90){
        move_and_turn(270, CELL_DISTANCE, 180);
        current_orientation = 180;
      }else if(current_orientation == 270){
        move_and_turn(90, CELL_DISTANCE, 90);
        current_orientation = 180;
      }
    }else if(DeltaY > 0){
      if(current_orientation == 270){
        move_robot(CELL_DISTANCE);
      }else if(current_orientation == 90){
        move_and_turn(180, CELL_DISTANCE, 180);
        current_orientation = 270;
      }else if(current_orientation == 180){
        move_and_turn(270, CELL_DISTANCE, 90);
        current_orientation = 270;
      }else if(current_orientation == 0){
        move_and_turn(90, CELL_DISTANCE, 90);
        current_orientation = 270;
      }
    }else if(DeltaY < 0){
      if(current_orientation == 90){
        move_robot(CELL_DISTANCE);
      }else if(current_orientation == 270){
        move_and_turn(180, CELL_DISTANCE, 180);
        current_orientation = 90;
      }else if(current_orientation == 180){
        move_and_turn(90, CELL_DISTANCE, 90);
        current_orientation = 90;
      }else if(current_orientation == 0){
        move_and_turn(270, CELL_DISTANCE, 90);
        current_orientation = 90;
      }
    }
    else{}
    current_position = tupla;
    idx = endIdx + 1;
  }
}