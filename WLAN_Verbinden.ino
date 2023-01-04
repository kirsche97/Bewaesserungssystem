void WLAN_Verbinden()
{
  if (Serial2.available())
  {
  dfd += char(Serial2.read());
  }
 
  if(dfd.substring(0,3)=="C:C")
  {
    if(dfd.substring((dfd.length()-1),dfd.length()) == "?")
    {
      String cmd = dfd.substring(3,6);
      String value = dfd.substring(6,dfd.length()-1);
      // NOTE : FOR TESTING
      Serial.println(cmd + " : " + value);
     
      if(cmd == "GET")
      {
        Serial2.print("xstr 0,250,600,40,0,BLACK,0,1,1,3,\"SCANNING...\"" + endChar);
        int n = WiFi.scanNetworks();    //Anzahl der Netzwerke
        String SSIDs = "";
        for (int i = 0; i < n; ++i) 
        {
          if(i > 0)SSIDs += "\r\n";
          SSIDs += WiFi.SSID(i);
          Serial.println(WiFi.SSID(i));
        }
        Serial2.print("cb_SSID.txt=\"" + String(n) + " Networks\"" + endChar);
        Serial2.print("cb_SSID.path=\"" + SSIDs + "\"" + endChar);
         
        Serial2.print("xstr 0,250,600,40,0,WHITE,0,1,1,3,\"SCANNING...\"" + endChar);
      }

      
      if(cmd == "CON")
      {
        Serial2.print("xstr 0,250,600,40,0,WHITE,0,1,1,3,\"Verbindung nicht möglich\"" + endChar);
        int colonLoc=value.indexOf("+");
        SSIDname = value.substring(0,colonLoc);
        SSIDpassword = value.substring(colonLoc+1);
        WiFi.begin(SSIDname.c_str(),SSIDpassword.c_str());
        int o=0;
         while(WiFi.status() != WL_CONNECTED &&  o != 20) 
          {
            delay(500);
            Serial.print(".");
            o++;
          }
          
          if (o==20)
          {
            Serial.print("Verbindung nicht möglich");
            Serial2.print("xstr 0,250,600,40,0,BLACK,0,1,1,3,\"Verbindung nicht möglich\"" + endChar);
          }
        Verbindungsstatus();
      }

      if(cmd == "DIS")
      {
        WiFi.disconnect();
        Serial2.print("t_passwort.txt=\"\"" + endChar);
        Serial2.print("t_benutzername.txt=\"\"" + endChar);
      }
     
      dfd="";
    }
  }
  
  if(WiFi.status() == WL_CONNECTED)
  {
    preferences.clear();
    preferences.putString("SSIDname",SSIDname);
    preferences.putString("SSIDpassword",SSIDpassword);
  }
}

  void Verbindungsstatus()
{
  digitalWrite(2,!(digitalRead(2)));
  // NOTE : STUFF HERE
  Serial.println(SSIDname + " : " + SSIDpassword);
  Serial.println(WiFi.localIP());
  Serial.println("STATUS : " + String(WiFi.status()));
  //  0: WL_IDLE --temp      if WiFibegin is called
  //  1: WL_NO_SSID_AVAIL    No SSID avail
  //  2: WL_SCAN_COMPLETE    Network SSID scan complete
  //  3: WL_CONNECTED        connected to SSID
  //  4: WL_CONNECT_FAILED   failed to connect to SSID
  //  5: WL_CONNECTION_LOST  connection is lost
  //  6: WL_DISCONNECTED     disconnected from network
  if(WiFi.status()==3)
  {
    Serial2.print("WLAN.txt=\"verbunden\"" + endChar);
    
  }
}

String SSID_Sammlung()
{
    AnzNetwerk = WiFi.scanNetworks();    //Anzahl der Netzwerke
    verfuegbare_Netzwerke = "";
        for (int i = 0; i < AnzNetwerk; ++i) 
        {
          
          verfuegbare_Netzwerke += WiFi.SSID(i);
          verfuegbare_Netzwerke += "§";
          Serial.println(WiFi.SSID(i));
        }
        return verfuegbare_Netzwerke;
}

boolean WLAN_Automatik()
{
  SSID_Sammlung(); 
  for (int i=0; i<AnzNetwerk;i++)
  {
    String temp = verfuegbare_Netzwerke.substring(0,verfuegbare_Netzwerke.indexOf("§")); 
    verfuegbare_Netzwerke=verfuegbare_Netzwerke.substring(verfuegbare_Netzwerke.indexOf("§")+1); //string immer weiter reduzieren 
    if(temp==preferences.getString("SSIDname",""))
    {
      SSIDpassword=preferences.getString("SSIDpassword","");    //Prüft ob wir eine Datei im eeprom haben, die den namen der SSID hat, Wenn nicht wird der String geleert zurückgegeben, sonst das Passwort
      if(SSIDpassword != "")
      {
        WiFi.begin(temp.c_str(), SSIDpassword.c_str());
        int o=0;
        while (WiFi.status() != WL_CONNECTED && o!=30) 
        {
          delay(100);
          Serial.println("Ich verbinde mich mit dem Internet...");
          o++;
        }
        if(WiFi.status() == WL_CONNECTED) 
        {
          Verbindungsstatus();
          Serial.println("Ich bin mit dem Internet verbunden!");
          return true;
        }
      }
    }
  }
  return false;
}
