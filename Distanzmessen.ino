long Distanzmessen()
{
  if (distsensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  dist =0;
  for(int i=0 ;i<=5; i++)
  {
    dist += distsensor.read();  
  }
  dist= dist /5;
  return dist;
 
}
