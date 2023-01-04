void Spuelen()
{  
    fahren();
    /*
  if(dfd=="BT2OF")
    {
    motorRichtung=0;
    dfd="";
    }*/
if (dfd.length()==5 && dfd.substring(0,1)=="D")
{
//Abfrage welche Düngestufe gespült werden soll
  if(dfd.length()==5 && dfd=="D1_SP")
    {
      digitalWrite(Steuerpin_Pumpe1,LOW);
      dfd="";
      delay(50);
    }
  
  if(dfd.length()==5 &&dfd=="D1_OF");
    {
      digitalWrite(Steuerpin_Pumpe1,HIGH);
      dfd="";
      delay(50);
    }  

  if(dfd.length()==5 &&dfd=="D2_SP")
    {
      digitalWrite(Steuerpin_Pumpe2,LOW);
      dfd="";
      delay(50);
    }

  if(dfd.length()==5 &&dfd=="D2_OF");
    {
      digitalWrite(Steuerpin_Pumpe2,HIGH);
      dfd="";
      delay(50);
    }      

  if(dfd.length()==5 &&dfd=="D3_SP")
    {
      digitalWrite(Steuerpin_Pumpe3,LOW);
      dfd="";
      delay(50);
    }

  if(dfd.length()==5 && dfd=="D3_OF");
    {
      digitalWrite(Steuerpin_Pumpe3,HIGH);
      dfd="";
      delay(50);
    }  

  if(dfd.length()==5 && dfd=="D4_SP")
    {
      digitalWrite(Steuerpin_Pumpe4,LOW);
      dfd="";
      delay(50);
    }
    
  if(dfd.length()==5 && dfd=="D4_OF");
    {
      digitalWrite(Steuerpin_Pumpe4,HIGH);
      dfd="";
      delay(50);
    }  

}


  

  // NOTE : ASYNC DELAY
  if(millis() > asyncDelay + delayLength)
  {
    if(asyncDelay > (4294967295-delayLength))
    {
      asyncDelay = (4294967295-asyncDelay) + (delayLength-(4294967295-asyncDelay));
    }else
    {
      asyncDelay+=delayLength;
    }
    //digitalWrite(13,!(digitalRead(13)));
    dfd="";
  }    
}
