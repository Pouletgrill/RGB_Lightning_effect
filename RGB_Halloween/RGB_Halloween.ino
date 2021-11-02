// HSV fade/bounce for Arduino - scruss.com - 2010/09/12
// Note that there's some legacy code left in here which seems to do nothing
// but should do no harm ...

// don't futz with these, illicit sums later
#define Indicator_LED 13
#define RED       9 // pin for red LED
#define GREEN    10 // pin for green - never explicitly referenced
#define BLUE     11 // pin for blue - never explicitly referenced

#define SIZE    255
#define DELAY    20
#define HUE_MAX  6.0
#define HUE_DELTA 0.01//0.01

//Thunder

#define BETWEEN 4000//2579
#define DURATION 43 
#define TIMES 7
//*********


//long deltas[3] = { 5, 6, 7 };
long rgb[3];
long rgb_tempo[3];
long rgbval;

//float rand;
boolean flag = false;

int val;
// for reasons unknown, if value !=0, the LED doesn't light. Hmm ...
// and saturation seems to be inverted
float hue=0.0, last_hue=0.0, saturation=1, value=1;

/*
chosen LED SparkFun sku: COM-09264
 has Max Luminosity (RGB): (2800, 6500, 1200)mcd
 so we normalize them all to 1200 mcd -
 R  250/600  =  107/256
 G  250/950  =   67/256
 B  250/250  =  256/256
 */
long bright[3] = { 107, 67, 256};
//long bright[3] = { 256, 256, 256};

long k, temp_value;
unsigned long lastTime = 0;
unsigned long lastTime_candle = 0;
int waitTime = 0;
int waitTime_candle = 100;


void setup () {
pinMode(Indicator_LED, OUTPUT);
digitalWrite(Indicator_LED, LOW);

Serial.begin(9600);/*Tempo*/
  
  randomSeed(analogRead(4));
  for (k=0; k<3; k++) {
    pinMode(RED + k, OUTPUT);
    rgb[k]=0;
    analogWrite(RED + k, rgb[k] * bright[k]/256);
  }
  //pinMode(MIC, INPUT);
  Serial.println("init fin");
}

void loop() {

/*TEST autogain code*/
//unsigned long startMillis= millis();  // Start of sample window
// Start of sample window

  /*hue += HUE_DELTA;
  if (hue > HUE_MAX) {
    hue=0.0;
  }*/
  
  //Thunder
  if (millis() - waitTime > lastTime)  // time for a new flash
  {
    // adjust timing params
    lastTime += waitTime;
    waitTime = 1000 + random(BETWEEN);
    
    rgb_tempo[0] = rgb[0];
    rgb_tempo[1] = rgb[1];
    rgb_tempo[2] = rgb[2];
    
    rgbval=HSV_to_RGB(1, 0, 1);
    //rgbval=HSV_to_RGB(hue, saturation, value);
    rgb[0] = (rgbval & 0x00FF0000) >> 16; // there must be better ways
    rgb[1] = (rgbval & 0x0000FF00) >> 8;
    rgb[2] = rgbval & 0x000000FF;

    //int occur = RandThunderOccur();
    //for (int i=0; i<occur ; i++)
    for (int i=0; i<random(TIMES) ; i++)
    {      
      for (k=0; k<3; k++) 
      { // for all three colours
        analogWrite(RED + k, rgb[k] * bright[k]/256);
        digitalWrite(Indicator_LED, HIGH);//Test
      }
      delay(30 + random(DURATION));
      for (k=0; k<3; k++) 
      { // for all three colours  
        analogWrite(RED + k, rgb_tempo[k] * bright[k]/256);
        digitalWrite(Indicator_LED, LOW);//Test
                
      }   
      delay(10 + random(DURATION)); 
    }

  }
  //Candle
  else if (millis() - lastTime_candle >= waitTime_candle)  // time for a new flash
  {
    lastTime_candle = millis();
    waitTime_candle = 100 + random(300);
    
    //hue de 50 a 30
    //rgbval=HSV_to_RGB(float(5+random(29))/100, 1, 1);//Old
    rgbval=HSV_to_RGB(float(10+random(26))/100, 1, 1);
    //rgbval=HSV_to_RGB(hue, saturation, value);
    rgb[0] = (rgbval & 0x00FF0000) >> 16; // there must be better ways
    rgb[1] = (rgbval & 0x0000FF00) >> 8;
    rgb[2] = rgbval & 0x000000FF;
    for (k=0; k<3; k++) { // for all three colours
      analogWrite(RED + k, rgb[k] * bright[k]/256);
    }
  }
}

long HSV_to_RGB( float h, float s, float v ) {
  /* modified from Alvy Ray Smith's site: http://www.alvyray.com/Papers/hsv2rgb.htm */
  // H is given on [0, 6]. S and V are given on [0, 1].
  // RGB is returned as a 24-bit long #rrggbb
  int i;
  float m, n, f;

  // not very elegant way of dealing with out of range: return black
  if ((s<0.0) || (s>1.0) || (v<0.0) || (v>1.0)) {
    return 0L;
  }

  if ((h < 0.0) || (h > 6.0)) {
    return long( v * 255 ) + long( v * 255 ) * 256 + long( v * 255 ) * 65536;
  }
  i = floor(h);
  f = h - i;
  if ( !(i&1) ) {
    f = 1 - f; // if i is even
  }
  m = v * (1 - s);
  n = v * (1 - s * f);
  switch (i) {
  case 6:
  case 0: 
    return long(v * 255 ) * 65536 + long( n * 255 ) * 256 + long( m * 255);
  case 1: 
    return long(n * 255 ) * 65536 + long( v * 255 ) * 256 + long( m * 255);
  case 2: 
    return long(m * 255 ) * 65536 + long( v * 255 ) * 256 + long( n * 255);
  case 3: 
    return long(m * 255 ) * 65536 + long( n * 255 ) * 256 + long( v * 255);
  case 4: 
    return long(n * 255 ) * 65536 + long( m * 255 ) * 256 + long( v * 255);
  case 5: 
    return long(v * 255 ) * 65536 + long( m * 255 ) * 256 + long( n * 255);
  }
} 

int RandThunderOccur() {
  int pourc = random(100+1);
  //Serial.println(pourc);
  
  //0%-20% = 20%
  if (pourc >0 & pourc <20)
  { Serial.println(1);return 1;} //1 tonerres
  
  //20%-50$ = 30%
  if (pourc >20 & pourc <50)
  { Serial.println(2);return 2;} //2 tonerres
  
  //50%-80% = 30%
  if (pourc >50 & pourc <80)
  { Serial.println(3);return 3;} //3 tonerres
  
  //80%-100% = 20%
  if (pourc >80)
  { Serial.println(111);return 4+random(3+1); }//entre 4 et 7 tonerres
    
  //return random(TIMES);
} 
