void vorwaertsFahren()
{
        digitalWrite(25, HIGH);
        
        for(int i = 0; i < 25; i++)
        {
        delayMicroseconds(500);
        digitalWrite(33,!(digitalRead(33)));
        delayMicroseconds(500);
        } 

        aktuellePosition++;
}


void rueckwaertsFahren()
{
        digitalWrite(25, LOW);
        for(int i = 0; i < 25; i++)
        {
        delayMicroseconds(500);
        digitalWrite(33,!(digitalRead(33)));
        delayMicroseconds(500);
        } 
        aktuellePosition++;    
  
}

void fahren()
{
  if(motorRichtung > 0)
    
    {// NOTE : if motor is on       
      if(motorRichtung == 1)
      {
       vorwaertsFahren();
      }
      if(motorRichtung == 2)
      {
        rueckwaertsFahren();
      }
    }
  
  // NOTE : COLLECT CHARACTERS FROM NEXTION DISPLAY
 if(Serial2.available())
 {
   dfd += char(Serial2.read());
   Serial.println(dfd);
 }
    if(dfd=="vor12")
    {
    motorRichtung=1;
    dfd="";
    delay(50);
    }
    
  if(dfd=="stp12")
    {
    motorRichtung=0;
    dfd="";
    delay(50);
    Serial2.print("n_Position.val=" + String(Distanzmessen()) + endChar);
    }
    
  if(dfd=="rck12")
    {
    motorRichtung=2;
    dfd="";
    delay(50);
    }
}
