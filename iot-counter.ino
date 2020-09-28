String data = "";
char temp = ' ';
int current_val=0;
int prev_inx = 0;
int parsed_data[10];

void setup() {
Serial.begin(9600);
}

void loop() {
  //While start
  while (Serial.available() > 0 ) {
    data = Serial.readString();
    Serial.println(data);
    //parser
    for(int i=0; i<data.length(); i++){
      temp = data[i];
      if(temp == ','){
        parsed_data[current_val]=data.substring(prev_inx, i).toInt();
        current_val++;
        prev_inx = i+1;
      }else if(i==(data.length()-1)){
        parsed_data[current_val]=data.substring(prev_inx, i+1).toInt();
        current_val++;
      }    
    }
    prev_inx = 0;
    current_val=0;
    //end parser
    //data array = parsed_data[]
    for(int i=0; i<10; i++){
    Serial.print("Channel ");
    Serial.print(i+1);
    Serial.print(": ");
    Serial.println(parsed_data[i]);
    }
  }
  //while end
}
