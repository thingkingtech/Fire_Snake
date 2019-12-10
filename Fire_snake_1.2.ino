//for T2 master 3.6 
#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>

#define NUM_LEDS_PER_STRIP 300
#define NUM_STRIPS 2

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
CRGB snakeArray[2100];
int ret[51];

// Pin layouts on the teensy 3:
// OctoWS2811: 2,14,7,8,6,20,21,5

void setup() {
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
  delay(100);
  fill_solid(snakeArray, 2100, CHSV(195,195,255));
  LEDS.setBrightness(255);
  breakUp();
  LEDS.show();
  delay(5000);//wait for all devices to be prepped
  //send start command
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  delay(100);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
}

void loop() {
  snakes();
  delay(1000);
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  delay(100);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  bastard();
  delay(1000);
  digitalWrite(22, HIGH);
  digitalWrite(23, HIGH);
  delay(100);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
}//end loop

//------------------------------------------------------------------------
//------------------------------------------------------------------------

void bastard (void){//5 min full cycle
  //ensure it is amber
  fill_solid(snakeArray, 2100, CHSV(255,255,255));
  LEDS.setBrightness(255);
  breakUp();
  LEDS.show();
  //wait 1 minutes
  delay(60000);

  //fade from Amber to slightly less Amber
  for(int i=0;i<=30;i++){//take 1 minute
    fill_solid(snakeArray, 2100, CHSV(255-i,255-i,255));
    delay(2000);
    breakUp();
    LEDS.show();
  }
  //wait for 1 minutes
  delay(60000);  

  //fade from slightly less Amber to even less amber
  //take 1 minutes
  for(int i=0;i<=30;i++){
    fill_solid(snakeArray, 2100, CHSV(225-i,225-i,255));
    delay(2000);
    breakUp();
    LEDS.show();
  }
  //wait for 1 minutes
  delay(60000);
  
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------

void snakes(void){
  int longLength = 100;
  int shortLength = 25;
  int shortLength2 = 25;
  int startPos=50;
  int count = 0;
  int skp=1;
  int skp2=1;
  int skp3=1;
  int hue=255;
  int hue2=195;
  int hueCounter = 1;
  //run long snakes slowly full circuit (trigger every 4th count)
  int trig=4;
  while(count<((2100*8)+6000)){
    count++;
    //introduce more snakes
    if((count>=2100*7) and (count<=2100*8)){
      snakeArray[checker((count+startPos))] = CRGB::White;
      snakeArray[checker((count+startPos)-shortLength)] = CHSV(hueChecker(hue),255,255);
      snakeArray[checker((count+startPos)+700)] = CRGB::White;
      snakeArray[checker((count+startPos)+700-shortLength)] = CHSV(hueChecker(hue),255,255);
      snakeArray[checker((count+startPos)+1400)] = CRGB::White;
      snakeArray[checker((count+startPos)+1400-shortLength)] = CHSV(hueChecker(hue),255,255);
    }
    //introduce small snakes
    if((count>=2100*6 and ((count<=2100*8)+1500)) and (skp==2 or skp==4)){
      snakeArray[checker(((count/2)+startPos))] = CRGB::Green;
      snakeArray[checker(((count/2)+startPos)-shortLength2)] = CHSV(hueChecker(hue),255,255);
      snakeArray[checker(((count/2)+startPos)+700)] = CRGB::Green;
      snakeArray[checker(((count/2)+startPos)+700-shortLength2)] = CHSV(hueChecker(hue),255,255);
      snakeArray[checker(((count/2)+startPos)+1400)] = CRGB::Green;
      snakeArray[checker(((count/2)+startPos)+1400-shortLength2)] = CHSV(hueChecker(hue),255,255);
    }
    //normal long snake
    if(skp==trig){
      skp=1;
      if(count==1440){
        hue=255;
      }
      if(hueCounter>=2){
        hueCounter=1;
        hue++;
        //fade to amber
        if(count>2100*8){
          hue=255;      
        }
      }else{
       hueCounter++;
       if(count>2100*8){
          hue=255;
       }
      }
      if(count<1440){
        if(skp2==6){
          skp2=1;
          hue2++;
        }else{
          skp2++;
        }
        snakeArray[checker((count/trig))] = CHSV(hue2,hue2,255);
        snakeArray[checker((count/trig)-longLength)] = CHSV(195,195,255);
        snakeArray[checker((count/trig)+700)] = CHSV(hue2,hue2,255);
        snakeArray[checker((count/trig)+700-longLength)] = CHSV(195,195,255);
        snakeArray[checker((count/trig)+1400)] = CHSV(hue2,hue2,255);
        snakeArray[checker((count/trig)+1400-longLength)] = CHSV(195,195,255);
      }else{
        snakeArray[checker((count/trig))] = CHSV(255,255,255);
        snakeArray[checker((count/trig)-longLength)] = CHSV(hueChecker(hue),255,255);
        snakeArray[checker((count/trig)+700)] = CHSV(255,255,255);
        snakeArray[checker((count/trig)+700-longLength)] = CHSV(hueChecker(hue),255,255);
        snakeArray[checker((count/trig)+1400)] = CHSV(255,255,255);
        snakeArray[checker((count/trig)+1400-longLength)] = CHSV(hueChecker(hue),255,255);
      }
    }else{
      skp++;
    }
    if((count>2100*8)+1200){
      if(skp3==10 and shortLength2>0){
        skp3=1;
        shortLength2--;
      }
      else{
        skp3++;
      }
      nm();
    }
    breakUp();
    LEDS.show();
  }
}//end snakes

//------------------------------------------------------------------------
//------------------------------------------------------------------------

//unused function, remanants of a more exciting time
void fancyFade(void){
  //start fading
  int count=0;
  while(count<((2100)+255)){//4200+255=5 min x 12 to make 57 -> adapted 2100+255=5 min
    delay(30);
    if(count<=255){
      LEDS.setBrightness(count);
    }
    for(int i=0; i<210;i++){
    snakeArray[checker(i)] = CHSV(hueChecker(count+i),255,255);
    snakeArray[checker(i+210)] = CHSV(hueChecker(210-(count+i)),255,255);
    snakeArray[checker(i+420)] = CHSV(hueChecker(count+i),255,255);
    snakeArray[checker(i+630)] = CHSV(hueChecker(210-(count+i)),255,255);
    snakeArray[checker(i+840)] = CHSV(hueChecker(count+i),255,255);
    snakeArray[checker(i+1050)] = CHSV(hueChecker(210-(count+i)),255,255);
    snakeArray[checker(i+1260)] = CHSV(hueChecker(count+i),255,255);
    snakeArray[checker(i+1470)] = CHSV(hueChecker(210-(count+i)),255,255);
    snakeArray[checker(i+1680)] = CHSV(hueChecker(count+i),255,255);
    snakeArray[checker(i+1890)] = CHSV(hueChecker(210-(count+i)),255,255);
    }

    //fade all to amber for b function
    if(count>=2100){//remember to change this too
      for(int i=0; i<210;i++){
        snakeArray[checker(i)] = CHSV(hueChecker2(hueChecker(count+8400)+count-8400),255,255);
        snakeArray[checker(i+210)] = CHSV(hueChecker2(hueChecker(210-(8400+i))+count-8400),255,255);
        snakeArray[checker(i+420)] = CHSV(hueChecker2(hueChecker(count+8400)+count-8400),255,255);
        snakeArray[checker(i+630)] = CHSV(hueChecker2(hueChecker(210-(8400+i))+count-8400),255,255);
        snakeArray[checker(i+840)] = CHSV(hueChecker2(hueChecker(count+8400)+count-8400),255,255);
        snakeArray[checker(i+1050)] = CHSV(hueChecker2(hueChecker(210-(8400+i))+count-8400),255,255);
        snakeArray[checker(i+1260)] = CHSV(hueChecker2(hueChecker(count+8400)+count-8400),255,255);
        snakeArray[checker(i+1470)] = CHSV(hueChecker2(hueChecker(210-(8400+i))+count-8400),255,255);
        snakeArray[checker(i+1680)] = CHSV(hueChecker2(hueChecker(count+8400)+count-8400),255,255);
        snakeArray[checker(i+1890)] = CHSV(hueChecker2(hueChecker(210-(8400+i))+count-8400),255,255);
      }
    }
    breakUp();
    LEDS.show();
    count++;
    delay(50);
  }
}//end fancyFade

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//ensures hue is withing bounds
int hueChecker(int l){
  while (l<0 || l>255){
    if (l<0){
      l=l+255;
    }
    else if (l > 255){
      l=l-255;
    }
  }
  return l;
}//end hueCchecker

//same as hueChecker but defaults hue to 255 if greater. Used to fade all to amber
int hueChecker2(int l){
  while (l<0 || l>255){
    if (l<0){
      l=l+255;
    }
    else if (l > 255){
      l=255;
    }
  }
  return l;
}//end hueCchecker

//ensures refrencing an LED withing bounds
int checker(int l){
  while (l<0 || l>2099){
    if (l<0){
      l=l+2100;
    }
    else if (l > 2099){
      l=l-2099;
    }
  }
  return l;
}//end checker

//unique to each micro. Only uses the part of the full snake array that it is responsible for
void breakUp(void){
  for(int pos = 0;pos<=299;pos++){
    leds[299-pos]=snakeArray[pos+900];
  }
  for(int pos = 300;pos<=599;pos++){
    leds[pos]=snakeArray[pos+900];
  }
}//end breakUp

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//unused function, remanants of a more exciting time
void sparkle(void){
  fill_solid(snakeArray, 2100, CRGB::Black);
  LEDS.setBrightness(255);
  breakUp();
  LEDS.show();
  delay(1000);
  int count=0;
  int cnt1=1;
  int cnt2=20;
  int cnt3=40;
  int cnt4=60;
  while(count<2100*1){
    delay(42);
    count++;
    //explosions at pos 0, 525, 1050, 1575
    if(cnt1<=30){
      for(int i=0;i<51;i++){
         lookUp(cnt1);
         snakeArray[checker(0+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
         snakeArray[checker(525+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
         snakeArray[checker(1050+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
         snakeArray[checker(1575+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
         snakeArray[checker(262+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
         snakeArray[checker(787+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
         snakeArray[checker(1312+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
         snakeArray[checker(1837+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
      }
    }


    //explosions at pos 100, 625, 1150, 1675
    if(cnt2<=30){
      for(int i=0;i<51;i++){
        lookUp(cnt2);
        snakeArray[checker(100+i)]=CHSV(255*ret[i],0,255*ret[i]);
        snakeArray[checker(625+i)]=CHSV(255*ret[i],0,255*ret[i]);
        snakeArray[checker(1150+i)]=CHSV(255*ret[i],0,255*ret[i]);
        snakeArray[checker(1675+i)]=CHSV(255*ret[i],0,255*ret[i]);
        snakeArray[checker(362+i)]=CHSV(255*ret[i],0,255*ret[i]);
        snakeArray[checker(887+i)]=CHSV(255*ret[i],0,255*ret[i]);
        snakeArray[checker(1412+i)]=CHSV(255*ret[i],0,255*ret[i]);
        snakeArray[checker(1937+i)]=CHSV(255*ret[i],0,255*ret[i]);
      }
    }

    //explosions at pos 200, 725, 1250, 1775
    if(cnt3<=30){
      for(int i=0;i<51;i++){
        lookUp(cnt3);
        snakeArray[checker(200+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
        snakeArray[checker(462+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
        lookUp(cnt3-2);
        snakeArray[checker(725+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
        snakeArray[checker(1014+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
        lookUp(cnt3-4);
        snakeArray[checker(1250+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
        snakeArray[checker(1512+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
        lookUp(cnt3-6);
        snakeArray[checker(1775+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
        snakeArray[checker(2037+i)]=CHSV(255*ret[i],255*ret[i],255*ret[i]);
      }
    }

    //explosions at pos 150, 450, 750, 1050, 1350, 1650, 1950
    if(cnt4<=30){
      for(int i=0;i<51;i++){
        lookUp(cnt4);
        snakeArray[checker(150+i)]=CHSV(175*ret[i],175*ret[i],255*ret[i]);
        snakeArray[checker(450+i)]=CHSV(175*ret[i],175*ret[i],255*ret[i]);
        snakeArray[checker(750+i)]=CHSV(175*ret[i],175*ret[i],255*ret[i]);
        snakeArray[checker(1050+i)]=CHSV(175*ret[i],175*ret[i],255*ret[i]);
        lookUp(cnt4-2);
        snakeArray[checker(1350+i)]=CHSV(175*ret[i],175*ret[i],255*ret[i]);
        snakeArray[checker(1650+i)]=CHSV(175*ret[i],175*ret[i],255*ret[i]);
        lookUp(cnt4-6);
        snakeArray[checker(1950+i)]=CHSV(175*ret[i],175*ret[i],255*ret[i]);
      }
    }


    if(cnt1>=100){
      cnt1=1;
    }else{
      cnt1++;
    }
    if(cnt2>=170){
      cnt2=1;
    }else{
      cnt2++;
    }
    if(cnt3>=75){
      cnt3=1;
    }else{
      cnt3++;
    }
    if(cnt4>=50){
      cnt4=1;
    }else{
      cnt4++;
    }
    breakUp();
    LEDS.show();
  }
}//end sparkle

//unused function, remanants of a more exciting time
//Frames for explosion/sparkles animations
void lookUp(int frame){
  if(frame==1){
    //          0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==2){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==3){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==4){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==5){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==6){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==7){
    //         1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==8){
    //         1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==9){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==10){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==11){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==12){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==13){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==14){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==15){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else if(frame==16){
    //          1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 M  25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1
    int ret1[]={0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }else{
    int ret1[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i =0;i<51;i++){
      ret[i]=ret1[i];
    }
  }
}//end lookUp

void nm(void){
  
}
