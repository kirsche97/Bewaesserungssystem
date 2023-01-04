void Topf()
{
   if(Serial2.available())
 {
   dfd += char(Serial2.read());
   Serial.println(dfd);
 }


String temp;

 // Serial2.print("n_Position.val=" + String(Distanzmessen()) + endChar);

if(geladen==false)
{
        for(int i = 1; i<=tAnzahl; i++)
    {
      temp+=String(i);
      temp+= "\r\n";
    }
    
    Serial2.print("n_Topfnr.path=\"" + temp + "\"" + endChar);
    Serial2.print("cb_copy.path=\"" + temp + "\"" + endChar);
    temp="";
        for(int i = 1; i<=sAnzahl; i++)
    {
      temp+=String(i);
      temp+= "\r\n";
    }
    Serial2.print("n_Sensornummer.path=\"" + temp + "\"" + endChar);
    geladen = true;
}

   if(dfd.length()==6 && dfd.substring(0,3)=="D:l")         //Werte für den ausgewählten Topf auf dem Display anzeigen
   {
    Topfnummer=dfd.substring(3,6).toInt()+1;                      //D:l001
    Serial.println("Topfnummer = " + String(Topfnummer));
    //Serial2.print("n_Topfnr.val=" + String(Pflanzen[Topfnummer-1].T_Nummer) + endChar);           //Da Array bei 0 beginnt -1
    Serial2.print("t_Position.val=" + String(Pflanzen[Topfnummer-1].Position) + endChar);
    Serial2.print("n_duenger.val=" + String(Pflanzen[Topfnummer-1].Duenger) + endChar);
    Serial2.print("n_bodenfeuchte.val=" + String(Pflanzen[Topfnummer-1].minFeuchte) + endChar);
    Serial2.print("n_Wasser.val=" + String(Pflanzen[Topfnummer-1].WasserMenge) + endChar);
    Serial2.print("n_Sensornummer.val=" + String(Pflanzen[Topfnummer-1].Sensornummer) + endChar);
    dfd="";
      Backup_erstellen(); // nur hier und nicht immer, da dann max. ein Topf verloren geht und dier Methode nicht jedes mal aufs neue durchlaufen werden muss; Zusätzliches Backup bei aufruf Home
   }

  if(dfd.length()==8 && dfd.substring(0,3)=="D:d")
  {
    Pflanzen[Topfnummer-1].Duenger=dfd.substring(3,8).toInt(); //nicht +1, da rückgabe auch Array
    dfd="";
  }

  if(dfd.length()==8 && dfd.substring(0,3)=="D:w")
  {
    Pflanzen[Topfnummer-1].WasserMenge=dfd.substring(3,8).toInt(); 
    dfd="";
  }

  if(dfd.length()==6 && dfd.substring(0,3)=="D:b")
  {
    Pflanzen[Topfnummer-1].minFeuchte=dfd.substring(3,6).toInt(); 
    dfd="";
  }


    if(dfd.length()==6 && dfd.substring(0,3)=="D:s")
  {
    Pflanzen[Topfnummer-1].Sensornummer=dfd.substring(3,6).toInt(); 
    dfd="";
  }
  
  if(dfd.length()== 3 && dfd.substring(0,3)=="D:P")
  {
    Pflanzen[Topfnummer-1].Position=Distanzmessen();
    Serial2.print("t_Position.val=" + String(Pflanzen[Topfnummer-1].Position) + endChar);
    dfd="";
  }
    if(dfd.length()==6 && dfd.substring(0,3)=="D:C")
  {
    int u = dfd.substring(3,6).toInt();
    Pflanzen[u].minFeuchte=Pflanzen[Topfnummer-1].minFeuchte;        //dfd übergibt die Topfnummer auf die kopiert werden soll
    Serial.println(String(Pflanzen[Topfnummer-1].minFeuchte));
    Serial.println(String(Pflanzen[u].minFeuchte));
    dfd="";
  }
fahren();


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
