// Author: Mike Katchmar
// E-mail: thachief@gmail.com
// 
// OK to use for non-commercial use.  If you do re-use or use as basis for your own 
// project please share video with Mike!  Maybe we can all learn something.  :)
// 
// Rev5 = first display in public - 11/23/2017 code complete


#include "FastLED.h"

#define DATA_PIN    6  // was 10 for "non production" units
#define BRIGHTNESS_PIN    A1
#define COLOR_ORDER RGB
#define LED_TYPE    WS2812
#define NUM_LEDS    78

#define FRAMES_PER_SECOND  120



// brightness control added 3/6/2016
// original limits:
//#define MASTER_BRIGHTNESS 30
#define MASTER_BRIGHTNESS 128 // Set the master brigtness value [should be greater then min_brightness value].
//#define MASTER_BRIGHTNESS 192 // Set the master brigtness value [should be greater then min_brightness value].
//#define MIN_BRIGHTNESS 30     // Set a minimum brightness level.
#define MIN_BRIGHTNESS 10     // Set a minimum brightness level.
int potValA;                  // Variable to store potentiometer A value (for brightness A)
uint8_t brightnessa;          // Mapped master brightness for color A based on potentiometer reading.

#define NUM_VIRTUAL_LEDS NUM_LEDS+1
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
CRGB leds[NUM_VIRTUAL_LEDS];



// Snowflake Structures
const uint8_t snowInnerRingCount = 18;

static const uint8_t snowInnerRingArray[] PROGMEM = 
  { 0, 1, 2, 3, 4, 5 , 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };


const uint8_t snowOuterRingCount = 24;

static const uint8_t snowOuterRingArray[] PROGMEM = 
  { 173, 18, 19, 20, 
    43, 44, 45, 46, 
    69, 70, 71, 72,
    95, 96, 97, 98,
    121, 122, 123, 124,
    147, 148, 149, 150};

const uint8_t flakeBranch1Count = 26;
const uint8_t flakeBranch2Count = 26;
const uint8_t flakeBranch3Count = 26;
const uint8_t flakeBranch4Count = 26;
const uint8_t flakeBranch5Count = 26;
const uint8_t flakeBranch6Count = 26;

// Full Sized Branch
static const uint8_t flakeBranch1Array[] PROGMEM = 
  { 0, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 1};
static const uint8_t flakeBranch2Array[] PROGMEM = 
  { 3, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 4};
static const uint8_t flakeBranch3Array[] PROGMEM = 
  { 6, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 7};
static const uint8_t flakeBranch4Array[] PROGMEM = 
  { 9, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 10};
static const uint8_t flakeBranch5Array[] PROGMEM = 
  { 12, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 13};
static const uint8_t flakeBranch6Array[] PROGMEM = 
  { 15, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 16};


const uint8_t flakeChervon1Count = 16;
const uint8_t flakeChervon2Count = 16;
const uint8_t flakeChervon3Count = 16;
const uint8_t flakeChervon4Count = 16;
const uint8_t flakeChervon5Count = 16;
const uint8_t flakeChervon6Count = 16;

// Just the "chevron" part of the branch
static const uint8_t flakeChervon1Array[] PROGMEM = 
  { 22, 23, 24, 25, 26, 27, 28, 29, 34, 35, 36, 37, 38, 39, 40, 41};
static const uint8_t flakeChervon2Array[] PROGMEM = 
  { 48, 49, 50, 51, 52, 53, 54, 55, 60, 61, 62, 63, 64, 65, 66, 67};
static const uint8_t flakeChervon3Array[] PROGMEM = 
  { 74, 75, 76, 77, 78, 79, 80, 81, 86, 87, 88, 89, 90, 91, 92, 93};
static const uint8_t flakeChervon4Array[] PROGMEM = 
  { 100, 101, 102, 103, 104, 105, 106, 107, 112, 113, 114, 115, 116, 117, 118, 119};
static const uint8_t flakeChervon5Array[] PROGMEM = 
  { 126, 127, 128, 129, 130, 131, 132, 133, 138, 139, 140, 141, 142, 143, 144, 145};
static const uint8_t flakeChervon6Array[] PROGMEM = 
  { 152, 153, 154, 155, 156, 157, 158, 159, 164, 165, 166, 167, 168, 169, 170, 171};

/* Are these still used?  
 *  
 */
  uint16_t holdTime = 60;  // Milliseconds to hold position before advancing.
  uint8_t spacing = 20;      // Sets pixel spacing. [Use 2 or greater]
  int8_t delta = 1;         // Sets forward or backwards direction amount. (Can be negative.)
  uint8_t width = 3;        // Can increase the number of pixels (width) of the chase. [1 or greater]
  
  uint8_t hue = 60;         // Starting color (marque_v3)
  
  boolean fadingTail = 1;   // Add fading tail? [1=true, 0=falue]
  //uint8_t fadeRate = 150;   // How fast to fade out tail. [0-255]
  uint8_t fadeRate = 70;   // How fast to fade out tail. [0-255]
  
  uint8_t hue2_shift = 60;  // Hue shift for secondary color.  Use 0 for no shift. [0-255]
  boolean DEBUG = 1;        // Print some info to serial monitor. [1=true, 0=falue]
  
  int16_t pos;              // Pixel position.
  int8_t advance;           // Stores the advance amount.
  uint8_t color;            // Stores a hue color.

  

// =============================================================================
//   setup routine
// =============================================================================
// put your setup code here, to run once:
void setup() {
  delay(1500); // 3 second delay for recovery
  Serial.begin(57600);
    
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };
//SimplePatternList gPatterns = { rainbow, sinelon, juggle, bpm };   // These are 4 patterns are very energetic, they should be used intermittenly within the whole solution, maybe 1 per 2 above?
//SimplePatternList gPatterns = { twinklefox };

// 9/4/2017 -- marque_v3 is failing with Snowflake.... look at hardcoded variables or change some other variable value?

// this list of patterns is good :)  9/27/2017
// consider using "bpm"
// SimplePatternList gPatterns = { wipe_top_to_bottom, branch_fade, wipe_left_to_right, confetti, center_ying_yang, TestGlitter, center_rainbow, snowflake_solid_shape_in_out_flip, gradient_fill_allbranches_snowflake, grow_allbranches_snowflake, trace_flake_allbranches_snowflake, fulldisplay_w_flash };

SimplePatternList gPatterns = { grow_allbranches_snowflake, center_ying_yang, wipe_left_to_right, TestGlitter, center_rainbow, trace_flake_allbranches_snowflake, snowflake_solid_shape_in_out_flip, confetti, branch_fade, wipe_top_to_bottom, gradient_fill_allbranches_snowflake, fulldisplay_w_flash };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns



// =============================================================================
//   loop routine 
// =============================================================================
// put your main code here, to run repeatedly:

void loop() {

    // brightness control 
//  potValA = analogRead(BRIGHTNESS_PIN);  // Read potentiometer B (for brightness).
  //Serial.println (potValA);
//  brightnessa = map(potValA, 0, 1023, MIN_BRIGHTNESS, MASTER_BRIGHTNESS);
      // Map value between min_brightness and MASTER brightness values.
      // Note: We are limiting the lowest possible brightness value to the
      // min_brightness value assigned up top.
  FastLED.setBrightness(MASTER_BRIGHTNESS);       
  //Serial.println (brightnessa);

  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 12 ) { nextPattern(); } // change patterns periodically

  EVERY_N_SECONDS(10){  // Demo: Change the hue every N seconds.
    hue = hue + random8(30,61);  // Shift the hue to something new.
  }
}



// =============================================================================
//  Sub-Routines
// =============================================================================


void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}




// ========================================
//  Routines for Pattern Display
// ========================================

void trace_inner_ring_snowflake() {

  uint8_t hue1 = random8(255);
  uint8_t offset2;
    
  for (int i=0; i<snowInnerRingCount; i++) {
    uint8_t offset = pgm_read_byte(&snowInnerRingArray[i]);
/*    
    Serial.print ("i=");
    Serial.print (i);
    Serial.print (" array location = ");
    Serial.println (offset);
*/    
    leds[offset] = CHSV(hue1, 255, 255);

    if ( (i+1)<snowInnerRingCount ) {
      offset2 = pgm_read_byte(&snowInnerRingArray[i+1]);
    }
    else {
      offset2 = pgm_read_byte(&snowInnerRingArray[i+1-snowInnerRingCount]);
    }
    leds[offset2] = CHSV(hue1, 255, 255);
        
    FastLED.show();
    leds[offset] = CRGB::Black;
    //delay(35);    
    delay(65);    
  }
}

void trace_outer_ring_snowflake() {

  uint8_t hue1 = random8(255);
  uint8_t offset2;
    
  for (int i=0; i<snowOuterRingCount; i++) {
    uint8_t offset = pgm_read_byte(&snowOuterRingArray[i]);
/*    
    Serial.print ("i=");
    Serial.print (i);
    Serial.print (" array location = ");
    Serial.println (offset);
*/    
    leds[offset] = CHSV(hue1, 255, 255);

    if ( (i+1)<snowOuterRingCount ) {
      offset2 = pgm_read_byte(&snowOuterRingArray[i+1]);
    }
    else {
      offset2 = pgm_read_byte(&snowOuterRingArray[i+1-snowOuterRingCount]);
    }
    leds[offset2] = CHSV(hue1, 255, 255);
        
    FastLED.show();
    leds[offset] = CRGB::Black;
    //delay(35);    
    delay(265);    
  }
}


void trace_flake_branch1_snowflake() {

  uint8_t hue1 = random8(255);
  uint8_t offset2;
    
  for (int i=0; i<flakeBranch1Count; i++) {
    uint8_t offset = pgm_read_byte(&flakeBranch1Array[i]);
    leds[offset] = CHSV(hue1, 255, 255);

    if ( (i+1)<flakeBranch1Count ) {
      offset2 = pgm_read_byte(&flakeBranch1Array[i+1]);
    }
    else {
      offset2 = pgm_read_byte(&flakeBranch1Array[i+1-flakeBranch1Count]);
    }
    leds[offset2] = CHSV(hue1, 255, 255);
        
    FastLED.show();
    leds[offset] = CRGB::Black;
    delay(35);    
  }
}


void trace_flake_branch2_snowflake() {

  uint8_t hue1 = random8(255);
  uint8_t offset2;
    
  for (int i=0; i<flakeBranch2Count; i++) {
    uint8_t offset = pgm_read_byte(&flakeBranch2Array[i]);
    leds[offset] = CHSV(hue1, 255, 255);

    if ( (i+1)<flakeBranch2Count ) {
      offset2 = pgm_read_byte(&flakeBranch2Array[i+1]);
    }
    else {
      offset2 = pgm_read_byte(&flakeBranch2Array[i+1-flakeBranch2Count]);
    }
    leds[offset2] = CHSV(hue1, 255, 255);
        
    FastLED.show();
    leds[offset] = CRGB::Black;
    delay(35);    
  }
}

void trace_flake_branch3_snowflake() {

  uint8_t hue1 = random8(255);
  uint8_t offset2;
    
  for (int i=0; i<flakeBranch3Count; i++) {
    uint8_t offset = pgm_read_byte(&flakeBranch3Array[i]);
    leds[offset] = CHSV(hue1, 255, 255);

    if ( (i+1)<flakeBranch3Count ) {
      offset2 = pgm_read_byte(&flakeBranch3Array[i+1]);
    }
    else {
      offset2 = pgm_read_byte(&flakeBranch3Array[i+1-flakeBranch3Count]);
    }
    leds[offset2] = CHSV(hue1, 255, 255);
        
    FastLED.show();
    leds[offset] = CRGB::Black;
    delay(35);    
  }
}

void trace_flake_branch4_snowflake() {

  uint8_t hue1 = random8(255);
  uint8_t offset2;
    
  for (int i=0; i<flakeBranch4Count; i++) {
    uint8_t offset = pgm_read_byte(&flakeBranch4Array[i]);
    leds[offset] = CHSV(hue1, 255, 255);

    if ( (i+1)<flakeBranch4Count ) {
      offset2 = pgm_read_byte(&flakeBranch4Array[i+1]);
    }
    else {
      offset2 = pgm_read_byte(&flakeBranch4Array[i+1-flakeBranch4Count]);
    }
    leds[offset2] = CHSV(hue1, 255, 255);
        
    FastLED.show();
    leds[offset] = CRGB::Black;
    delay(35);    
  }
}

void trace_flake_branch5_snowflake() {

  uint8_t hue1 = random8(255);
  uint8_t offset2;
    
  for (int i=0; i<flakeBranch5Count; i++) {
    uint8_t offset = pgm_read_byte(&flakeBranch5Array[i]);
    leds[offset] = CHSV(hue1, 255, 255);

    if ( (i+1)<flakeBranch5Count ) {
      offset2 = pgm_read_byte(&flakeBranch5Array[i+1]);
    }
    else {
      offset2 = pgm_read_byte(&flakeBranch5Array[i+1-flakeBranch5Count]);
    }
    leds[offset2] = CHSV(hue1, 255, 255);
        
    FastLED.show();
    leds[offset] = CRGB::Black;
    delay(35);    
  }
}

void trace_flake_branch6_snowflake() {

  uint8_t hue1 = random8(255);
  uint8_t offset2;
    
  for (int i=0; i<flakeBranch6Count; i++) {
    uint8_t offset = pgm_read_byte(&flakeBranch6Array[i]);
    leds[offset] = CHSV(hue1, 255, 255);

    if ( (i+1)<flakeBranch6Count ) {
      offset2 = pgm_read_byte(&flakeBranch6Array[i+1]);
    }
    else {
      offset2 = pgm_read_byte(&flakeBranch6Array[i+1-flakeBranch6Count]);
    }
    leds[offset2] = CHSV(hue1, 255, 255);
        
    FastLED.show();
    leds[offset] = CRGB::Black;
    delay(35);    
  }
}


// 2 pixel wide trace
void trace_flake_allbranches_snowflake() {

  uint8_t hue1 = random8(255);
  uint8_t offset2_br1;
  uint8_t offset2_br2;
  uint8_t offset2_br3;
  uint8_t offset2_br4;
  uint8_t offset2_br5;
  uint8_t offset2_br6;

  FastLED.clear();
    
  for (int i=0; i<flakeBranch1Count; i++) {
    uint8_t offset_br1 = pgm_read_byte(&flakeBranch1Array[i]);
    uint8_t offset_br2 = pgm_read_byte(&flakeBranch2Array[i]);
    uint8_t offset_br3 = pgm_read_byte(&flakeBranch3Array[i]);
    uint8_t offset_br4 = pgm_read_byte(&flakeBranch4Array[i]);
    uint8_t offset_br5 = pgm_read_byte(&flakeBranch5Array[i]);
    uint8_t offset_br6 = pgm_read_byte(&flakeBranch6Array[i]);

    // set one LED per branch
    leds[offset_br1] = CHSV(hue1, 255, 255);
    leds[offset_br2] = CHSV(hue1, 255, 255);
    leds[offset_br3] = CHSV(hue1, 255, 255);
    leds[offset_br4] = CHSV(hue1, 255, 255);
    leds[offset_br5] = CHSV(hue1, 255, 255);
    leds[offset_br6] = CHSV(hue1, 255, 255);

    // now set one LED as a look-ahead per branch
    if ( (i+1)<flakeBranch1Count ) {
      offset2_br1 = pgm_read_byte(&flakeBranch1Array[i+1]);
      offset2_br2 = pgm_read_byte(&flakeBranch2Array[i+1]);
      offset2_br3 = pgm_read_byte(&flakeBranch3Array[i+1]);
      offset2_br4 = pgm_read_byte(&flakeBranch4Array[i+1]);
      offset2_br5 = pgm_read_byte(&flakeBranch5Array[i+1]);
      offset2_br6 = pgm_read_byte(&flakeBranch6Array[i+1]);
    }
    else {
      offset2_br1 = pgm_read_byte(&flakeBranch1Array[i+1-flakeBranch1Count]);
      offset2_br2 = pgm_read_byte(&flakeBranch2Array[i+1-flakeBranch2Count]);
      offset2_br3 = pgm_read_byte(&flakeBranch3Array[i+1-flakeBranch3Count]);
      offset2_br4 = pgm_read_byte(&flakeBranch4Array[i+1-flakeBranch4Count]);
      offset2_br5 = pgm_read_byte(&flakeBranch5Array[i+1-flakeBranch5Count]);
      offset2_br6 = pgm_read_byte(&flakeBranch6Array[i+1-flakeBranch6Count]);
    }
    leds[offset2_br1] = CHSV(hue1, 255, 255);
    leds[offset2_br2] = CHSV(hue1, 255, 255);
    leds[offset2_br3] = CHSV(hue1, 255, 255);
    leds[offset2_br4] = CHSV(hue1, 255, 255);
    leds[offset2_br5] = CHSV(hue1, 255, 255);
    leds[offset2_br6] = CHSV(hue1, 255, 255);
        
    FastLED.show();
    leds[offset_br1] = CRGB::Black;
    leds[offset_br2] = CRGB::Black;
    leds[offset_br3] = CRGB::Black;
    leds[offset_br4] = CRGB::Black;
    leds[offset_br5] = CRGB::Black;
    leds[offset_br6] = CRGB::Black;
    delay(40);    
  }
}

void gradient_fill_allbranches_snowflake() {

  uint8_t offset_br1;
  uint8_t offset_br2;
  uint8_t offset_br3;
  uint8_t offset_br4;
  uint8_t offset_br5;
  uint8_t offset_br6;

  uint8_t hue1 = random8(255);
  uint8_t hue2 = hue1 + random8(30,61);
  uint8_t offset2;

  // this is where the magic happens -- define temp array to hold values
  CRGB grad[flakeBranch1Count];  // A place to save the gradient colors. (Don't edit this)
  // fill "holder array - grad" then copy values to each branch in the array below
  fill_gradient (grad, 0, CHSV(hue1, 255, 255), flakeBranch1Count, CHSV(hue2, 255, 255), SHORTEST_HUES);


  // display each pixel one at a time
  for( int i = 0; i < flakeBranch1Count; i++){

    offset_br1 = pgm_read_byte(&flakeBranch1Array[i]);
    offset_br2 = pgm_read_byte(&flakeBranch2Array[i]);
    offset_br3 = pgm_read_byte(&flakeBranch3Array[i]);
    offset_br4 = pgm_read_byte(&flakeBranch4Array[i]);
    offset_br5 = pgm_read_byte(&flakeBranch5Array[i]);
    offset_br6 = pgm_read_byte(&flakeBranch6Array[i]);
    
    leds[offset_br1] = grad[i];
    leds[offset_br2] = grad[i];
    leds[offset_br3] = grad[i];
    leds[offset_br4] = grad[i];
    leds[offset_br5] = grad[i];
    leds[offset_br6] = grad[i];
    
    delay(18);
    FastLED.show();
  }
  //FastLED.clear();
}


void snowflake_solid_shape_in_out_flip() {

  const uint8_t COUNT_RING = 42;
  uint8_t offset_ring_inner;
  uint8_t offset_ring_outer;
  uint8_t offset_chevron1;
  uint8_t offset_chevron2;
  uint8_t offset_chevron3;
  uint8_t offset_chevron4;
  uint8_t offset_chevron5;
  uint8_t offset_chevron6;

  CRGB color_code1;
  CRGB color_code2;


  for( int run_number = 0; run_number < 10; run_number++){

    // change the color every cycle through
    // color options:
    //  https://github.com/FastLED/FastLED/blob/03d12093a92ee2b64fabb03412aa0c3e4f6384dd/pixeltypes.h
    switch (run_number) {
    case 0:
      color_code1 = CRGB::Blue;
      color_code2 = CRGB::FireBrick;
      break;
    case 1:
      color_code1 = CRGB::Yellow;
      color_code2 = CRGB::DeepPink;
      break;
    case 2:
      color_code1 = CRGB::SpringGreen;
      color_code2 = CRGB::Silver;
      break;
    case 3:
      color_code1 = CRGB::Green; 
      color_code2 = CRGB::Fuchsia;
      break;
    case 4:
      color_code1 = CRGB::Tomato;
      color_code2 = CRGB::DarkSlateGray;
      break;
    case 5:
      color_code1 = CRGB::DarkRed; 
      color_code2 = CRGB::Goldenrod; // white
      break;
    case 6:
      color_code1 = CRGB::Honeydew;    // MistyRose;  // white
      color_code2 = CRGB::DarkOrange;
      break;
    case 7:
      color_code1 = CRGB::DeepPink;
      color_code2 = CRGB::Red;
      break;
    case 8:
      color_code1 = CRGB::Cyan;
      color_code2 = CRGB::Crimson;
      break;
    case 9:
      color_code1 = CRGB::Amethyst;
      color_code2 = CRGB::Yellow;   
      break;
    default:
      // keep other color
      break;
    }
    
    // STEP 1:
    // Build Inner Ring
    for( int i = 0; i < snowInnerRingCount; i++){
      offset_ring_inner = pgm_read_byte(&snowInnerRingArray[i]);    
      leds[offset_ring_inner] = color_code1;
    }
    for( int i = 0; i < snowOuterRingCount; i++){
      offset_ring_outer = pgm_read_byte(&snowOuterRingArray[i]);    
      leds[offset_ring_outer] = color_code1;
    }
    // Build 6 Chervons
    for( int j = 0; j < flakeChervon1Count; j++){
      offset_chevron1 = pgm_read_byte(&flakeChervon1Array[j]);
      offset_chevron2 = pgm_read_byte(&flakeChervon2Array[j]);
      offset_chevron3 = pgm_read_byte(&flakeChervon3Array[j]);
      offset_chevron4 = pgm_read_byte(&flakeChervon4Array[j]);
      offset_chevron5 = pgm_read_byte(&flakeChervon5Array[j]);
      offset_chevron6 = pgm_read_byte(&flakeChervon6Array[j]);
      leds[offset_chevron1] = color_code1;
      leds[offset_chevron2] = color_code1;
      leds[offset_chevron3] = color_code1;
      leds[offset_chevron4] = color_code1;
      leds[offset_chevron5] = color_code1;
      leds[offset_chevron6] = color_code1;
    }
    
    FastLED.show();
    delay(700);
    FastLED.clear();

    // STEP2 2:
    // Wipe & Show Other View
    // Build 6 Branches
    for( int j = 0; j < flakeBranch1Count; j++){
      offset_chevron1 = pgm_read_byte(&flakeBranch1Array[j]);
      offset_chevron2 = pgm_read_byte(&flakeBranch2Array[j]);
      offset_chevron3 = pgm_read_byte(&flakeBranch3Array[j]);
      offset_chevron4 = pgm_read_byte(&flakeBranch4Array[j]);
      offset_chevron5 = pgm_read_byte(&flakeBranch5Array[j]);
      offset_chevron6 = pgm_read_byte(&flakeBranch6Array[j]);
      leds[offset_chevron1] = color_code2;
      leds[offset_chevron2] = color_code2;
      leds[offset_chevron3] = color_code2;
      leds[offset_chevron4] = color_code2;
      leds[offset_chevron5] = color_code2;
      leds[offset_chevron6] = color_code2;
    }
      
    FastLED.show();
    delay(700);
    FastLED.clear();
  }
  
}



void grow_allbranches_snowflake() {

  const uint8_t NUM_PHASES = 35;

  uint8_t offset_br1_led1, offset_br1_led2, offset_br1_led3, offset_br1_led4;
  uint8_t offset_br2_led1, offset_br2_led2, offset_br2_led3, offset_br2_led4;
  uint8_t offset_br3_led1, offset_br3_led2, offset_br3_led3, offset_br3_led4;
  uint8_t offset_br4_led1, offset_br4_led2, offset_br4_led3, offset_br4_led4;
  uint8_t offset_br5_led1, offset_br5_led2, offset_br5_led3, offset_br5_led4;
  uint8_t offset_br6_led1, offset_br6_led2, offset_br6_led3, offset_br6_led4;

  // pick a random color
  uint8_t hue1 = random8(255);


  for (int i=0; i<NUM_PHASES; i++) {
    
    switch (i) {
    case 0:
      // turn all LEDs off??
      break;

    case 1:
      // build the flake (2 wide, 1 tall)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[0]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[25]);
      leds[offset_br1_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[0]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[25]);
      leds[offset_br2_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[0]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[25]);
      leds[offset_br3_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[0]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[25]);
      leds[offset_br4_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[0]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[25]);
      leds[offset_br5_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[0]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[25]);
      leds[offset_br6_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led2] = CHSV(hue1+10*i, 255, 255);

      break;
    
    case 2:
      // build the flake (2 wide, 2 tall)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[1]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[24]);
      leds[offset_br1_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[1]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[24]);
      leds[offset_br2_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[1]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[24]);
      leds[offset_br3_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[1]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[24]);
      leds[offset_br4_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led2] = CHSV(hue1+10*i, 255, 255);
      
      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[1]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[24]);
      leds[offset_br5_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[1]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[24]);
      leds[offset_br6_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led2] = CHSV(hue1+10*i, 255, 255);

      break;
    
    case 3:
      // build the flake (2 wide, 3 tall)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[2]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[23]);
      leds[offset_br1_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[2]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[23]);
      leds[offset_br2_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[2]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[23]);
      leds[offset_br3_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[2]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[23]);
      leds[offset_br4_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[2]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[23]);
      leds[offset_br5_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[2]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[23]);
      leds[offset_br6_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led2] = CHSV(hue1+10*i, 255, 255);

      break;
    
    case 4:
      // build the flake (2 wide, 4 tall)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[3]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[22]);
      leds[offset_br1_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[3]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[22]);
      leds[offset_br2_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[3]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[22]);
      leds[offset_br3_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[3]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[22]);
      leds[offset_br4_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[3]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[22]);
      leds[offset_br5_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[3]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[22]);
      leds[offset_br6_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led2] = CHSV(hue1+10*i, 255, 255);

      break;
      
    case 5:
      // build the flake (2 wide, 5 tall)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[10]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[15]);
      leds[offset_br1_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[10]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[15]);
      leds[offset_br2_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[10]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[15]);
      leds[offset_br3_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[10]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[15]);
      leds[offset_br4_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[10]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[15]);
      leds[offset_br5_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[10]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[15]);
      leds[offset_br6_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led2] = CHSV(hue1+10*i, 255, 255);

      break;
      
    case 6:
      // build the flake (2 wide, 5 tall, 1 high each side branch)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[4]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[9]);
      offset_br1_led3 = pgm_read_byte(&flakeBranch1Array[16]);
      offset_br1_led4 = pgm_read_byte(&flakeBranch1Array[21]);
      leds[offset_br1_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[4]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[9]);
      offset_br2_led3 = pgm_read_byte(&flakeBranch2Array[16]);
      offset_br2_led4 = pgm_read_byte(&flakeBranch2Array[21]);
      leds[offset_br2_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[4]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[9]);
      offset_br3_led3 = pgm_read_byte(&flakeBranch3Array[16]);
      offset_br3_led4 = pgm_read_byte(&flakeBranch3Array[21]);
      leds[offset_br3_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[4]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[9]);
      offset_br4_led3 = pgm_read_byte(&flakeBranch4Array[16]);
      offset_br4_led4 = pgm_read_byte(&flakeBranch4Array[21]);
      leds[offset_br4_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[4]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[9]);
      offset_br5_led3 = pgm_read_byte(&flakeBranch5Array[16]);
      offset_br5_led4 = pgm_read_byte(&flakeBranch5Array[21]);
      leds[offset_br5_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[4]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[9]);
      offset_br6_led3 = pgm_read_byte(&flakeBranch6Array[16]);
      offset_br6_led4 = pgm_read_byte(&flakeBranch6Array[21]);
      leds[offset_br6_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led4] = CHSV(hue1+10*i, 255, 255);

      break;

    case 7:
      // build the flake (2 wide, 6 tall, 1 high each side branch)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[11]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[14]);
      leds[offset_br1_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led2] = CHSV(hue1+10*i, 255, 255);
      
      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[11]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[14]);
      leds[offset_br2_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led2] = CHSV(hue1+10*i, 255, 255);
      
      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[11]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[14]);
      leds[offset_br3_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led2] = CHSV(hue1+10*i, 255, 255);
      
      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[11]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[14]);
      leds[offset_br4_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led2] = CHSV(hue1+10*i, 255, 255);
      
      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[11]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[14]);
      leds[offset_br5_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led2] = CHSV(hue1+10*i, 255, 255);
      
      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[11]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[14]);
      leds[offset_br6_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led2] = CHSV(hue1+10*i, 255, 255);
      
      break;

    case 8:
      // build the flake (2 wide, 6 tall, 2 high pix each side branch)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[5]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[8]);
      offset_br1_led3 = pgm_read_byte(&flakeBranch1Array[17]);
      offset_br1_led4 = pgm_read_byte(&flakeBranch1Array[20]);
      leds[offset_br1_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[5]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[8]);
      offset_br2_led3 = pgm_read_byte(&flakeBranch2Array[17]);
      offset_br2_led4 = pgm_read_byte(&flakeBranch2Array[20]);
      leds[offset_br2_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[5]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[8]);
      offset_br3_led3 = pgm_read_byte(&flakeBranch3Array[17]);
      offset_br3_led4 = pgm_read_byte(&flakeBranch3Array[20]);
      leds[offset_br3_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[5]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[8]);
      offset_br4_led3 = pgm_read_byte(&flakeBranch4Array[17]);
      offset_br4_led4 = pgm_read_byte(&flakeBranch4Array[20]);
      leds[offset_br4_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[5]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[8]);
      offset_br5_led3 = pgm_read_byte(&flakeBranch5Array[17]);
      offset_br5_led4 = pgm_read_byte(&flakeBranch5Array[20]);
      leds[offset_br5_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[5]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[8]);
      offset_br6_led3 = pgm_read_byte(&flakeBranch6Array[17]);
      offset_br6_led4 = pgm_read_byte(&flakeBranch6Array[20]);
      leds[offset_br6_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led4] = CHSV(hue1+10*i, 255, 255);

      break;
      
    case 9:
      // build the flake (2 wide, 7 tall, 2 high each side branch)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[12]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[13]);
      leds[offset_br1_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[12]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[13]);
      leds[offset_br2_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[12]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[13]);
      leds[offset_br3_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[12]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[13]);
      leds[offset_br4_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[12]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[13]);
      leds[offset_br5_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led2] = CHSV(hue1+10*i, 255, 255);

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[12]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[13]);
      leds[offset_br6_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led2] = CHSV(hue1+10*i, 255, 255);

      break;

    case 10:
      // build the flake (2 wide, 6 tall, 3 high pix each side branch)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[6]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[7]);
      offset_br1_led3 = pgm_read_byte(&flakeBranch1Array[18]);
      offset_br1_led4 = pgm_read_byte(&flakeBranch1Array[19]);
      leds[offset_br1_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br1_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[6]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[7]);
      offset_br2_led3 = pgm_read_byte(&flakeBranch2Array[18]);
      offset_br2_led4 = pgm_read_byte(&flakeBranch2Array[19]);
      leds[offset_br2_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br2_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[6]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[7]);
      offset_br3_led3 = pgm_read_byte(&flakeBranch3Array[18]);
      offset_br3_led4 = pgm_read_byte(&flakeBranch3Array[19]);
      leds[offset_br3_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br3_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[6]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[7]);
      offset_br4_led3 = pgm_read_byte(&flakeBranch4Array[18]);
      offset_br4_led4 = pgm_read_byte(&flakeBranch4Array[19]);
      leds[offset_br4_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br4_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[6]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[7]);
      offset_br5_led3 = pgm_read_byte(&flakeBranch5Array[18]);
      offset_br5_led4 = pgm_read_byte(&flakeBranch5Array[19]);
      leds[offset_br5_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br5_led4] = CHSV(hue1+10*i, 255, 255);

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[6]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[7]);
      offset_br6_led3 = pgm_read_byte(&flakeBranch6Array[18]);
      offset_br6_led4 = pgm_read_byte(&flakeBranch6Array[19]);
      leds[offset_br6_led1] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led2] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led3] = CHSV(hue1+10*i, 255, 255);
      leds[offset_br6_led4] = CHSV(hue1+10*i, 255, 255);

      break;

  /* Desconstruct it */
    case 21:
      // de-build the flake (2 wide, 6 tall, 2 high pix each side branch)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[6]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[7]);
      offset_br1_led3 = pgm_read_byte(&flakeBranch1Array[18]);
      offset_br1_led4 = pgm_read_byte(&flakeBranch1Array[19]);
      leds[offset_br1_led1] = CRGB::Black;
      leds[offset_br1_led2] = CRGB::Black;
      leds[offset_br1_led3] = CRGB::Black;
      leds[offset_br1_led4] = CRGB::Black;

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[6]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[7]);
      offset_br2_led3 = pgm_read_byte(&flakeBranch2Array[18]);
      offset_br2_led4 = pgm_read_byte(&flakeBranch2Array[19]);
      leds[offset_br2_led1] = CRGB::Black;
      leds[offset_br2_led2] = CRGB::Black;
      leds[offset_br2_led3] = CRGB::Black;
      leds[offset_br2_led4] = CRGB::Black;

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[6]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[7]);
      offset_br3_led3 = pgm_read_byte(&flakeBranch3Array[18]);
      offset_br3_led4 = pgm_read_byte(&flakeBranch3Array[19]);
      leds[offset_br3_led1] = CRGB::Black;
      leds[offset_br3_led2] = CRGB::Black;
      leds[offset_br3_led3] = CRGB::Black;
      leds[offset_br3_led4] = CRGB::Black;

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[6]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[7]);
      offset_br4_led3 = pgm_read_byte(&flakeBranch4Array[18]);
      offset_br4_led4 = pgm_read_byte(&flakeBranch4Array[19]);
      leds[offset_br4_led1] = CRGB::Black;
      leds[offset_br4_led2] = CRGB::Black;
      leds[offset_br4_led3] = CRGB::Black;
      leds[offset_br4_led4] = CRGB::Black;

      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[6]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[7]);
      offset_br5_led3 = pgm_read_byte(&flakeBranch5Array[18]);
      offset_br5_led4 = pgm_read_byte(&flakeBranch5Array[19]);
      leds[offset_br5_led1] = CRGB::Black;
      leds[offset_br5_led2] = CRGB::Black;
      leds[offset_br5_led3] = CRGB::Black;
      leds[offset_br5_led4] = CRGB::Black;

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[6]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[7]);
      offset_br6_led3 = pgm_read_byte(&flakeBranch6Array[18]);
      offset_br6_led4 = pgm_read_byte(&flakeBranch6Array[19]);
      leds[offset_br6_led1] = CRGB::Black;
      leds[offset_br6_led2] = CRGB::Black;
      leds[offset_br6_led3] = CRGB::Black;
      leds[offset_br6_led4] = CRGB::Black;

      break;

    case 22:
      // de-build the flake (2 wide, 6 tall, 2 high each side branch)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[12]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[13]);
      leds[offset_br1_led1] = CRGB::Black;
      leds[offset_br1_led2] = CRGB::Black;

      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[12]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[13]);
      leds[offset_br2_led1] = CRGB::Black;
      leds[offset_br2_led2] = CRGB::Black;

      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[12]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[13]);
      leds[offset_br3_led1] = CRGB::Black;
      leds[offset_br3_led2] = CRGB::Black;

      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[12]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[13]);
      leds[offset_br4_led1] = CRGB::Black;
      leds[offset_br4_led2] = CRGB::Black;

      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[12]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[13]);
      leds[offset_br5_led1] = CRGB::Black;
      leds[offset_br5_led2] = CRGB::Black;

      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[12]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[13]);
      leds[offset_br6_led1] = CRGB::Black;
      leds[offset_br6_led2] = CRGB::Black;

      break;

    case 23:
      // de-build the flake (2 wide, 6 tall, 1 high each side branch)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[5]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[8]);
      offset_br1_led3 = pgm_read_byte(&flakeBranch1Array[17]);
      offset_br1_led4 = pgm_read_byte(&flakeBranch1Array[20]);
      leds[offset_br1_led1] = CRGB::Black;
      leds[offset_br1_led2] = CRGB::Black;
      leds[offset_br1_led3] = CRGB::Black;
      leds[offset_br1_led4] = CRGB::Black;
      
      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[5]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[8]);
      offset_br2_led3 = pgm_read_byte(&flakeBranch2Array[17]);
      offset_br2_led4 = pgm_read_byte(&flakeBranch2Array[20]);
      leds[offset_br2_led1] = CRGB::Black;
      leds[offset_br2_led2] = CRGB::Black;
      leds[offset_br2_led3] = CRGB::Black;
      leds[offset_br2_led4] = CRGB::Black;
      
      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[5]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[8]);
      offset_br3_led3 = pgm_read_byte(&flakeBranch3Array[17]);
      offset_br3_led4 = pgm_read_byte(&flakeBranch3Array[20]);
      leds[offset_br3_led1] = CRGB::Black;
      leds[offset_br3_led2] = CRGB::Black;
      leds[offset_br3_led3] = CRGB::Black;
      leds[offset_br3_led4] = CRGB::Black;
      
      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[5]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[8]);
      offset_br4_led3 = pgm_read_byte(&flakeBranch4Array[17]);
      offset_br4_led4 = pgm_read_byte(&flakeBranch4Array[20]);
      leds[offset_br4_led1] = CRGB::Black;
      leds[offset_br4_led2] = CRGB::Black;
      leds[offset_br4_led3] = CRGB::Black;
      leds[offset_br4_led4] = CRGB::Black;
      
      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[5]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[8]);
      offset_br5_led3 = pgm_read_byte(&flakeBranch5Array[17]);
      offset_br5_led4 = pgm_read_byte(&flakeBranch5Array[20]);
      leds[offset_br5_led1] = CRGB::Black;
      leds[offset_br5_led2] = CRGB::Black;
      leds[offset_br5_led3] = CRGB::Black;
      leds[offset_br5_led4] = CRGB::Black;
      
      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[5]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[8]);
      offset_br6_led3 = pgm_read_byte(&flakeBranch6Array[17]);
      offset_br6_led4 = pgm_read_byte(&flakeBranch6Array[20]);
      leds[offset_br6_led1] = CRGB::Black;
      leds[offset_br6_led2] = CRGB::Black;
      leds[offset_br6_led3] = CRGB::Black;
      leds[offset_br6_led4] = CRGB::Black;
      
      break;

    case 24:
      // de-build the flake (2 wide, 5 tall, 1 high each side branch)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[11]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[14]);
      leds[offset_br1_led1] = CRGB::Black;
      leds[offset_br1_led2] = CRGB::Black;
      
      // de-build the flake (2 wide, 5 tall, 1 high each side branch)
      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[11]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[14]);
      leds[offset_br2_led1] = CRGB::Black;
      leds[offset_br2_led2] = CRGB::Black;
      
      // de-build the flake (2 wide, 5 tall, 1 high each side branch)
      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[11]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[14]);
      leds[offset_br3_led1] = CRGB::Black;
      leds[offset_br3_led2] = CRGB::Black;
      
      // de-build the flake (2 wide, 5 tall, 1 high each side branch)
      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[11]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[14]);
      leds[offset_br4_led1] = CRGB::Black;
      leds[offset_br4_led2] = CRGB::Black;
      
      // de-build the flake (2 wide, 5 tall, 1 high each side branch)
      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[11]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[14]);
      leds[offset_br5_led1] = CRGB::Black;
      leds[offset_br5_led2] = CRGB::Black;
      
      // de-build the flake (2 wide, 5 tall, 1 high each side branch)
      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[11]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[14]);
      leds[offset_br6_led1] = CRGB::Black;
      leds[offset_br6_led2] = CRGB::Black;
      
      break;

    case 25:
      // de-build the flake (2 wide, 5 tall)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[4]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[9]);
      offset_br1_led3 = pgm_read_byte(&flakeBranch1Array[16]);
      offset_br1_led4 = pgm_read_byte(&flakeBranch1Array[21]);
      leds[offset_br1_led1] = CRGB::Black;
      leds[offset_br1_led2] = CRGB::Black;
      leds[offset_br1_led3] = CRGB::Black;
      leds[offset_br1_led4] = CRGB::Black;
      
      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[4]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[9]);
      offset_br2_led3 = pgm_read_byte(&flakeBranch2Array[16]);
      offset_br2_led4 = pgm_read_byte(&flakeBranch2Array[21]);
      leds[offset_br2_led1] = CRGB::Black;
      leds[offset_br2_led2] = CRGB::Black;
      leds[offset_br2_led3] = CRGB::Black;
      leds[offset_br2_led4] = CRGB::Black;
      
      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[4]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[9]);
      offset_br3_led3 = pgm_read_byte(&flakeBranch3Array[16]);
      offset_br3_led4 = pgm_read_byte(&flakeBranch3Array[21]);
      leds[offset_br3_led1] = CRGB::Black;
      leds[offset_br3_led2] = CRGB::Black;
      leds[offset_br3_led3] = CRGB::Black;
      leds[offset_br3_led4] = CRGB::Black;
      
      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[4]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[9]);
      offset_br4_led3 = pgm_read_byte(&flakeBranch4Array[16]);
      offset_br4_led4 = pgm_read_byte(&flakeBranch4Array[21]);
      leds[offset_br4_led1] = CRGB::Black;
      leds[offset_br4_led2] = CRGB::Black;
      leds[offset_br4_led3] = CRGB::Black;
      leds[offset_br4_led4] = CRGB::Black;
      
      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[4]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[9]);
      offset_br5_led3 = pgm_read_byte(&flakeBranch5Array[16]);
      offset_br5_led4 = pgm_read_byte(&flakeBranch5Array[21]);
      leds[offset_br5_led1] = CRGB::Black;
      leds[offset_br5_led2] = CRGB::Black;
      leds[offset_br5_led3] = CRGB::Black;
      leds[offset_br5_led4] = CRGB::Black;
      
      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[4]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[9]);
      offset_br6_led3 = pgm_read_byte(&flakeBranch6Array[16]);
      offset_br6_led4 = pgm_read_byte(&flakeBranch6Array[21]);
      leds[offset_br6_led1] = CRGB::Black;
      leds[offset_br6_led2] = CRGB::Black;
      leds[offset_br6_led3] = CRGB::Black;
      leds[offset_br6_led4] = CRGB::Black;
      
      break;

    case 26:
      // de-build the flake (2 wide, 4 tall)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[10]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[15]);
      leds[offset_br1_led1] = CRGB::Black;
      leds[offset_br1_led2] = CRGB::Black;
      
      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[10]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[15]);
      leds[offset_br2_led1] = CRGB::Black;
      leds[offset_br2_led2] = CRGB::Black;
      
      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[10]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[15]);
      leds[offset_br3_led1] = CRGB::Black;
      leds[offset_br3_led2] = CRGB::Black;
      
      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[10]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[15]);
      leds[offset_br4_led1] = CRGB::Black;
      leds[offset_br4_led2] = CRGB::Black;
      
      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[10]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[15]);
      leds[offset_br5_led1] = CRGB::Black;
      leds[offset_br5_led2] = CRGB::Black;
      
      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[10]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[15]);
      leds[offset_br6_led1] = CRGB::Black;
      leds[offset_br6_led2] = CRGB::Black;
      
      break;
      
    case 27:
      // de-build the flake (2 wide, 3 tall)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[3]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[22]);
      leds[offset_br1_led1] = CRGB::Black;
      leds[offset_br1_led2] = CRGB::Black;
      
      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[3]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[22]);
      leds[offset_br2_led1] = CRGB::Black;
      leds[offset_br2_led2] = CRGB::Black;
      
      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[3]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[22]);
      leds[offset_br3_led1] = CRGB::Black;
      leds[offset_br3_led2] = CRGB::Black;
      
      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[3]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[22]);
      leds[offset_br4_led1] = CRGB::Black;
      leds[offset_br4_led2] = CRGB::Black;
      
      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[3]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[22]);
      leds[offset_br5_led1] = CRGB::Black;
      leds[offset_br5_led2] = CRGB::Black;
      
      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[3]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[22]);
      leds[offset_br6_led1] = CRGB::Black;
      leds[offset_br6_led2] = CRGB::Black;
      
      break;

    case 28:
      // de-build the flake (2 wide, 2 tall)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[2]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[23]);
      leds[offset_br1_led1] = CRGB::Black;
      leds[offset_br1_led2] = CRGB::Black;
      
      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[2]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[23]);
      leds[offset_br2_led1] = CRGB::Black;
      leds[offset_br2_led2] = CRGB::Black;
      
      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[2]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[23]);
      leds[offset_br3_led1] = CRGB::Black;
      leds[offset_br3_led2] = CRGB::Black;
      
      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[2]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[23]);
      leds[offset_br4_led1] = CRGB::Black;
      leds[offset_br4_led2] = CRGB::Black;
      
      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[2]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[23]);
      leds[offset_br5_led1] = CRGB::Black;
      leds[offset_br5_led2] = CRGB::Black;
      
      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[2]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[23]);
      leds[offset_br6_led1] = CRGB::Black;
      leds[offset_br6_led2] = CRGB::Black;
      
      break;

    case 29:
      // de-build the flake (2 wide, 1 tall)
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[1]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[24]);
      leds[offset_br1_led1] = CRGB::Black;
      leds[offset_br1_led2] = CRGB::Black;
      
      offset_br2_led1 = pgm_read_byte(&flakeBranch2Array[1]);
      offset_br2_led2 = pgm_read_byte(&flakeBranch2Array[24]);
      leds[offset_br2_led1] = CRGB::Black;
      leds[offset_br2_led2] = CRGB::Black;
      
      offset_br3_led1 = pgm_read_byte(&flakeBranch3Array[1]);
      offset_br3_led2 = pgm_read_byte(&flakeBranch3Array[24]);
      leds[offset_br3_led1] = CRGB::Black;
      leds[offset_br3_led2] = CRGB::Black;
      
      offset_br4_led1 = pgm_read_byte(&flakeBranch4Array[1]);
      offset_br4_led2 = pgm_read_byte(&flakeBranch4Array[24]);
      leds[offset_br4_led1] = CRGB::Black;
      leds[offset_br4_led2] = CRGB::Black;
      
      offset_br5_led1 = pgm_read_byte(&flakeBranch5Array[1]);
      offset_br5_led2 = pgm_read_byte(&flakeBranch5Array[24]);
      leds[offset_br5_led1] = CRGB::Black;
      leds[offset_br5_led2] = CRGB::Black;
      
      offset_br6_led1 = pgm_read_byte(&flakeBranch6Array[1]);
      offset_br6_led2 = pgm_read_byte(&flakeBranch6Array[24]);
      leds[offset_br6_led1] = CRGB::Black;
      leds[offset_br6_led2] = CRGB::Black;
      
      break;
    
    case 30:
      // de-build the flake (2 wide, 0 tall)
      FastLED.clear();

      /*  ORIGINAL WAY -->
      offset_br1_led1 = pgm_read_byte(&flakeBranch1Array[0]);
      offset_br1_led2 = pgm_read_byte(&flakeBranch1Array[25]);
      leds[offset_br1_led1] = CRGB::Black;
      leds[offset_br1_led2] = CRGB::Black;
      */
      break;
    
    default:
      // if nothing else matches, do the default
      // HOLD it solid for a few cycles... maybe later we can add some animation.  
      // Now we'll hold it solid for cycles 11-20
      break;
    }  

    FastLED.show();
    delay (60);
  } 
  
}

void center_ying_yang() {
  const uint8_t NUM_PHASES = 19;

  // pick a random color
  CRGB color_code1;
  CRGB color_code2;

  // clear the slate
  FastLED.clear();

  for (int j=0; j<20; j++) {

    // change the color every cycle through
    // color options:
    //  https://github.com/FastLED/FastLED/blob/03d12093a92ee2b64fabb03412aa0c3e4f6384dd/pixeltypes.h
    switch (j) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      color_code1 = CRGB::Aqua;
      color_code2 = CRGB::FireBrick;
      break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      color_code1 = CRGB::Yellow;
      color_code2 = CRGB::DeepPink;
      break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
      color_code1 = CRGB::Red;
      color_code2 = CRGB::Green;
      break;
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
      color_code1 = CRGB::White;
      color_code2 = CRGB::Black;
      break;
    default:
      break;
    }
          
    for (int i=0; i<NUM_PHASES; i++) {

      switch (i) {
      case 0:
        // turn all LEDs off
        //FastLED.clear();
        break;
  
      case 1:
        leds[0] = color_code1;
        leds[20] = color_code1;
        
        leds[9] = color_code2;
        leds[98] = color_code2;
        break;
  
      case 2:
        leds[1] = color_code1;
        leds[43] = color_code1;
        
        leds[10] = color_code2;
        leds[121] = color_code2;
        break;
  
      case 3:
        leds[2] = color_code1;
        leds[44] = color_code1;
        leds[45] = color_code1;
        
        leds[11] = color_code2;
        leds[122] = color_code2;
        leds[123] = color_code2;
        break;
        
      case 4:
        leds[3] = color_code1;
        leds[46] = color_code1;
        
        leds[12] = color_code2;
        leds[124] = color_code2;
        break;
  
      case 5:
        leds[4] = color_code1;
        leds[69] = color_code1;
        
        leds[13] = color_code2;
        leds[147] = color_code2;
        break;
  
      case 6:
        leds[5] = color_code1;
        leds[70] = color_code1;
        leds[71] = color_code1;
        
        leds[14] = color_code2;
        leds[148] = color_code2;
        leds[149] = color_code2;
        break;
  
      case 7:
        leds[6] = color_code1;
        leds[72] = color_code1;
        
        leds[15] = color_code2;
        leds[150] = color_code2;
        break;
  
      case 8:
        leds[7] = color_code1;
        leds[95] = color_code1;
        
        leds[16] = color_code2;
        leds[173] = color_code2;
        break;
  
      case 9:
        leds[8] = color_code1;
        leds[96] = color_code1;
        leds[97] = color_code1;
        
        leds[17] = color_code2;
        leds[18] = color_code2;
        leds[19] = color_code2;
        break;
  
      case 10:
        leds[0] = color_code2;
        leds[20] = color_code2;

        leds[9] = color_code1;
        leds[98] = color_code1;
        break;
  
      case 11:
        leds[1] = color_code2;
        leds[43] = color_code2;

        leds[10] = color_code1;
        leds[121] = color_code1;
        break;
  
      case 12:
        leds[2] = color_code2;
        leds[44] = color_code2;
        leds[45] = color_code2;

        leds[11] = color_code1;
        leds[122] = color_code1;
        leds[123] = color_code1;
        break;
  
      case 13:
        leds[3] = color_code2;
        leds[46] = color_code2;

        leds[12] = color_code1;
        leds[124] = color_code1;
        break;
  
      case 14:
        leds[4] = color_code2;
        leds[69] = color_code2;
        
        leds[13] = color_code1;
        leds[147] = color_code1;
        break;
  
      case 15:
        leds[5] = color_code2;
        leds[70] = color_code2;
        leds[71] = color_code2;
        
        leds[14] = color_code1;
        leds[148] = color_code1;
        leds[149] = color_code1;
        break;
  
      case 16:
        leds[6] = color_code2;
        leds[72] = color_code2;
        
        leds[15] = color_code1;
        leds[150] = color_code1;
        break;
  
      case 17:
        leds[7] = color_code2;
        leds[95] = color_code2;

        leds[16] = color_code1;
        leds[173] = color_code1;
        break;
  
      case 18:
        leds[8] = color_code2;
        leds[96] = color_code2;
        leds[97] = color_code2;
        
        leds[17] = color_code1;
        leds[18] = color_code1;
        leds[19] = color_code1;
        break;
  
      default:
        // if nothing else matches, do the default
        break;
      }  
  
      FastLED.show();
      delay (30);
      fadeToBlackBy( leds, NUM_LEDS, 55);
    }
    // end display loop 
  }
  // end loop for number of displays loop
}


void center_rainbow() {

  const uint8_t NUM_PHASES = 18;

  // pick a random color
  uint8_t hue1 = random8(255);

  // clear the slate
  FastLED.clear();

  for (int j=0; j<25; j++) {
      
    for (int i=1; i<NUM_PHASES+1; i++) {

      // always update the color during each iteration
      hue1+=3;

      switch (i) {
      case 0:
        // turn all LEDs off
        //FastLED.clear();
        break;
  
      case 1:
        leds[0] = CHSV(hue1, 255, 255);
        leds[20] = CHSV(hue1, 255, 255);
        break;
  
      case 2:
        leds[1] = CHSV(hue1, 255, 255);
        leds[43] = CHSV(hue1, 255, 255);
        break;
  
      case 3:
        leds[2] = CHSV(hue1, 255, 255);
        leds[44] = CHSV(hue1, 255, 255);
        leds[45] = CHSV(hue1, 255, 255);
        break;
        
      case 4:
        leds[3] = CHSV(hue1, 255, 255);
        leds[46] = CHSV(hue1, 255, 255);
        break;
  
      case 5:
        leds[4] = CHSV(hue1, 255, 255);
        leds[69] = CHSV(hue1, 255, 255);
        break;
  
      case 6:
        leds[5] = CHSV(hue1, 255, 255);
        leds[70] = CHSV(hue1, 255, 255);
        leds[71] = CHSV(hue1, 255, 255);
        break;
  
      case 7:
        leds[6] = CHSV(hue1, 255, 255);
        leds[72] = CHSV(hue1, 255, 255);
        break;
  
      case 8:
        leds[7] = CHSV(hue1, 255, 255);
        leds[95] = CHSV(hue1, 255, 255);
        break;
  
      case 9:
        leds[8] = CHSV(hue1, 255, 255);
        leds[96] = CHSV(hue1, 255, 255);
        leds[97] = CHSV(hue1, 255, 255);
        break;
  
      case 10:
        leds[9] = CHSV(hue1, 255, 255);
        leds[98] = CHSV(hue1, 255, 255);
        break;
  
      case 11:
        leds[10] = CHSV(hue1, 255, 255);
        leds[121] = CHSV(hue1, 255, 255);
        break;
  
      case 12:
        leds[11] = CHSV(hue1, 255, 255);
        leds[122] = CHSV(hue1, 255, 255);
        leds[123] = CHSV(hue1, 255, 255);
        break;
  
      case 13:
        leds[12] = CHSV(hue1, 255, 255);
        leds[124] = CHSV(hue1, 255, 255);
        break;
  
      case 14:
        leds[13] = CHSV(hue1, 255, 255);
        leds[147] = CHSV(hue1, 255, 255);
        break;
  
      case 15:
        leds[14] = CHSV(hue1, 255, 255);
        leds[148] = CHSV(hue1, 255, 255);
        leds[149] = CHSV(hue1, 255, 255);
        break;
  
      case 16:
        leds[15] = CHSV(hue1, 255, 255);
        leds[150] = CHSV(hue1, 255, 255);
        break;
  
      case 17:
        leds[16] = CHSV(hue1, 255, 255);
        leds[173] = CHSV(hue1, 255, 255);
        break;
  
      case 18:
        leds[17] = CHSV(hue1, 255, 255);
        leds[18] = CHSV(hue1, 255, 255);
        leds[19] = CHSV(hue1, 255, 255);
        break;
  
  
      default:
        // if nothing else matches, do the default
        // HOLD it solid for a few cycles... maybe later we can add some animation.  
        // Now we'll hold it solid for cycles 11-20
        break;
      }  
  
      FastLED.show();
      delay (20);
    }
    // end display loop 
  }
  // end loop for number of displays loop
}


void wipe_left_to_right() {

  const uint8_t NUM_PHASES = 40;


  // pick a random color
  uint8_t hue1 = random8(255);

  // clear the slate
  FastLED.clear();

  for (int j=0; j<8; j++) {
      
    for (int i=1; i<NUM_PHASES+1; i++) {

      // always update the color during each iteration
      //hue1+=3;  // significant color change start to end
      hue1+=1;

      switch (i) {
      case 0:
        // turn all LEDs off
        //FastLED.clear();
        break;
  
      case 1:
        leds[57] = CHSV(hue1, 255, 255);
        leds[58] = CHSV(hue1, 255, 255);
        break;
  
      case 2:
        leds[52] = CHSV(hue1, 255, 255);
        leds[63] = CHSV(hue1, 255, 255);
        break;
  
      case 3:
        leds[51] = CHSV(hue1, 255, 255);
        leds[53] = CHSV(hue1, 255, 255);
        leds[56] = CHSV(hue1, 255, 255);
        leds[59] = CHSV(hue1, 255, 255);
        leds[62] = CHSV(hue1, 255, 255);
        leds[64] = CHSV(hue1, 255, 255);
        break;
        
      case 4:
        leds[50] = CHSV(hue1, 255, 255);
        leds[54] = CHSV(hue1, 255, 255);
        leds[61] = CHSV(hue1, 255, 255);
        leds[65] = CHSV(hue1, 255, 255);
        break;

      case 5:
        leds[37] = CHSV(hue1, 255, 255);
        leds[38] = CHSV(hue1, 255, 255);
        leds[55] = CHSV(hue1, 255, 255);
        leds[60] = CHSV(hue1, 255, 255);
        leds[77] = CHSV(hue1, 255, 255);
        leds[78] = CHSV(hue1, 255, 255);
        break;
  
      case 6:
        leds[49] = CHSV(hue1, 255, 255);
        leds[66] = CHSV(hue1, 255, 255);
        break;
  
      case 7:
        leds[36] = CHSV(hue1, 255, 255);
        leds[39] = CHSV(hue1, 255, 255);
        leds[48] = CHSV(hue1, 255, 255);
        leds[67] = CHSV(hue1, 255, 255);
        leds[76] = CHSV(hue1, 255, 255);
        leds[79] = CHSV(hue1, 255, 255);
        break;
  
      case 8:
        leds[32] = CHSV(hue1, 255, 255);
        leds[35] = CHSV(hue1, 255, 255);
        leds[80] = CHSV(hue1, 255, 255);
        leds[83] = CHSV(hue1, 255, 255);
        break;
  
      case 9:
        leds[31] = CHSV(hue1, 255, 255);
        leds[33] = CHSV(hue1, 255, 255);
        leds[40] = CHSV(hue1, 255, 255);
        leds[47] = CHSV(hue1, 255, 255);
        leds[68] = CHSV(hue1, 255, 255);
        leds[75] = CHSV(hue1, 255, 255);
        leds[82] = CHSV(hue1, 255, 255);
        leds[84] = CHSV(hue1, 255, 255);
        break;
        
      case 10:
        leds[30] = CHSV(hue1, 255, 255);
        leds[34] = CHSV(hue1, 255, 255);
        leds[45] = CHSV(hue1, 255, 255);
        leds[70] = CHSV(hue1, 255, 255);
        leds[81] = CHSV(hue1, 255, 255);
        leds[85] = CHSV(hue1, 255, 255);
        break;
  
      case 11:
        leds[41] = CHSV(hue1, 255, 255);
        leds[44] = CHSV(hue1, 255, 255);
        leds[46] = CHSV(hue1, 255, 255);
        leds[69] = CHSV(hue1, 255, 255);
        leds[71] = CHSV(hue1, 255, 255);
        leds[74] = CHSV(hue1, 255, 255);
        break;
  
      case 12:
        leds[29] = CHSV(hue1, 255, 255);
        leds[2] = CHSV(hue1, 255, 255);
        leds[5] = CHSV(hue1, 255, 255);
        leds[86] = CHSV(hue1, 255, 255);
        break;

      case 13:
        leds[28] = CHSV(hue1, 255, 255);
        leds[22] = CHSV(hue1, 255, 255);
        leds[42] = CHSV(hue1, 255, 255);
        leds[3] = CHSV(hue1, 255, 255);
        leds[4] = CHSV(hue1, 255, 255);
        leds[73] = CHSV(hue1, 255, 255);
        leds[93] = CHSV(hue1, 255, 255);
        leds[87] = CHSV(hue1, 255, 255);
        break;

      case 14:
        leds[27] = CHSV(hue1, 255, 255);
        leds[23] = CHSV(hue1, 255, 255);
        leds[21] = CHSV(hue1, 255, 255);
        leds[43] = CHSV(hue1, 255, 255);
        leds[72] = CHSV(hue1, 255, 255);
        leds[94] = CHSV(hue1, 255, 255);
        leds[92] = CHSV(hue1, 255, 255);
        leds[88] = CHSV(hue1, 255, 255);
        break;
  
      case 15:
        leds[26] = CHSV(hue1, 255, 255);
        leds[24] = CHSV(hue1, 255, 255);
        leds[20] = CHSV(hue1, 255, 255);
        leds[1] = CHSV(hue1, 255, 255);
        leds[6] = CHSV(hue1, 255, 255);
        leds[95] = CHSV(hue1, 255, 255);
        leds[91] = CHSV(hue1, 255, 255);
        leds[89] = CHSV(hue1, 255, 255);
        break;

      case 16:
        leds[25] = CHSV(hue1, 255, 255);
        leds[19] = CHSV(hue1, 255, 255);
        leds[0] = CHSV(hue1, 255, 255);
        leds[7] = CHSV(hue1, 255, 255);
        leds[96] = CHSV(hue1, 255, 255);
        leds[90] = CHSV(hue1, 255, 255);
        break;

/* Center Position */
      case 17:
        leds[17] = CHSV(hue1, 255, 255);
        leds[8] = CHSV(hue1, 255, 255);
        break;

      case 18:
        leds[168] = CHSV(hue1, 255, 255);
        leds[18] = CHSV(hue1, 255, 255);
        leds[16] = CHSV(hue1, 255, 255);
        leds[9] = CHSV(hue1, 255, 255);
        leds[97] = CHSV(hue1, 255, 255);
        leds[103] = CHSV(hue1, 255, 255);
        break;
  
      case 19:
        leds[167] = CHSV(hue1, 255, 255);
        leds[169] = CHSV(hue1, 255, 255);
        leds[173] = CHSV(hue1, 255, 255);
        leds[15] = CHSV(hue1, 255, 255);
        leds[10] = CHSV(hue1, 255, 255);
        leds[98] = CHSV(hue1, 255, 255);
        leds[102] = CHSV(hue1, 255, 255);
        leds[104] = CHSV(hue1, 255, 255);
        break;

      case 20:
        leds[166] = CHSV(hue1, 255, 255);
        leds[170] = CHSV(hue1, 255, 255);
        leds[172] = CHSV(hue1, 255, 255);
        leds[150] = CHSV(hue1, 255, 255);
        leds[121] = CHSV(hue1, 255, 255);
        leds[99] = CHSV(hue1, 255, 255);
        leds[101] = CHSV(hue1, 255, 255);
        leds[105] = CHSV(hue1, 255, 255);
        break;

      case 21:
        leds[165] = CHSV(hue1, 255, 255);
        leds[171] = CHSV(hue1, 255, 255);
        leds[151] = CHSV(hue1, 255, 255);
        leds[13] = CHSV(hue1, 255, 255);
        leds[12] = CHSV(hue1, 255, 255);
        leds[120] = CHSV(hue1, 255, 255);
        leds[100] = CHSV(hue1, 255, 255);
        leds[106] = CHSV(hue1, 255, 255);
        break;

      case 22:
        leds[164] = CHSV(hue1, 255, 255);
        leds[14] = CHSV(hue1, 255, 255);
        leds[11] = CHSV(hue1, 255, 255);
        leds[107] = CHSV(hue1, 255, 255);
        break;

      case 23:
        leds[152] = CHSV(hue1, 255, 255);
        leds[149] = CHSV(hue1, 255, 255);
        leds[147] = CHSV(hue1, 255, 255);
        leds[124] = CHSV(hue1, 255, 255);
        leds[122] = CHSV(hue1, 255, 255);
        leds[119] = CHSV(hue1, 255, 255);
        break;
  
      case 24:
        leds[163] = CHSV(hue1, 255, 255);
        leds[159] = CHSV(hue1, 255, 255);
        leds[148] = CHSV(hue1, 255, 255);
        leds[123] = CHSV(hue1, 255, 255);
        leds[112] = CHSV(hue1, 255, 255);
        leds[108] = CHSV(hue1, 255, 255);
        break;
      
      case 25:
        leds[162] = CHSV(hue1, 255, 255);
        leds[160] = CHSV(hue1, 255, 255);
        leds[153] = CHSV(hue1, 255, 255);
        leds[146] = CHSV(hue1, 255, 255);
        leds[125] = CHSV(hue1, 255, 255);
        leds[118] = CHSV(hue1, 255, 255);
        leds[111] = CHSV(hue1, 255, 255);
        leds[109] = CHSV(hue1, 255, 255);
        break;
        
      case 26:
        leds[161] = CHSV(hue1, 255, 255);
        leds[158] = CHSV(hue1, 255, 255);
        leds[113] = CHSV(hue1, 255, 255);
        leds[110] = CHSV(hue1, 255, 255);
        break;

      case 27:
        leds[157] = CHSV(hue1, 255, 255);
        leds[154] = CHSV(hue1, 255, 255);
        leds[145] = CHSV(hue1, 255, 255);
        leds[126] = CHSV(hue1, 255, 255);
        leds[117] = CHSV(hue1, 255, 255);
        leds[114] = CHSV(hue1, 255, 255);
        break;
     
      case 28:
        leds[144] = CHSV(hue1, 255, 255);
        leds[127] = CHSV(hue1, 255, 255);
        break;
      
      case 29:
        leds[156] = CHSV(hue1, 255, 255);
        leds[155] = CHSV(hue1, 255, 255);
        leds[138] = CHSV(hue1, 255, 255);
        leds[133] = CHSV(hue1, 255, 255);
        leds[116] = CHSV(hue1, 255, 255);
        leds[115] = CHSV(hue1, 255, 255);
        break;
  
      case 30:
        leds[143] = CHSV(hue1, 255, 255);
        leds[139] = CHSV(hue1, 255, 255);
        leds[132] = CHSV(hue1, 255, 255);
        leds[128] = CHSV(hue1, 255, 255);
        break;
  
      case 31:
        leds[142] = CHSV(hue1, 255, 255);
        leds[140] = CHSV(hue1, 255, 255);
        leds[137] = CHSV(hue1, 255, 255);
        leds[134] = CHSV(hue1, 255, 255);
        leds[131] = CHSV(hue1, 255, 255);
        leds[129] = CHSV(hue1, 255, 255);
        break;

      case 32:
        leds[141] = CHSV(hue1, 255, 255);
        leds[130] = CHSV(hue1, 255, 255);
        break;

      case 33:
        leds[136] = CHSV(hue1, 255, 255);
        leds[135] = CHSV(hue1, 255, 255);
        break;

  
      default:
        // if nothing else matches, do the default
        // HOLD it solid for a few cycles... maybe later we can add some animation.  
        // Now we'll hold it solid for cycles 11-20
        break;
      }  
  
      FastLED.show();
      delay (20);
    }
    // end display loop 
  }
  // end loop for number of displays loop
}



void wipe_top_to_bottom() {

  const uint8_t NUM_PHASES = 40;


  // pick a random color
  uint8_t hue1 = random8(255);

  // clear the slate
  FastLED.clear();

  for (int j=0; j<8; j++) {
      
    for (int i=1; i<NUM_PHASES+1; i++) {

      // always update the color during each iteration
      //hue1+=3;  // significant color change start to end
      hue1+=1;

      switch (i) {
      case 0:
        // turn all LEDs off
        //FastLED.clear();
        break;
  
      case 1:
        leds[26] = CHSV(hue1, 255, 255);
        leds[167] = CHSV(hue1, 255, 255);
        break;
  
      case 2:
        leds[31] = CHSV(hue1, 255, 255);
        leds[25] = CHSV(hue1, 255, 255);
        leds[168] = CHSV(hue1, 255, 255);
        leds[162] = CHSV(hue1, 255, 255);
        break;

      case 3:
        leds[32] = CHSV(hue1, 255, 255);
        leds[27] = CHSV(hue1, 255, 255);
        leds[166] = CHSV(hue1, 255, 255);
        leds[161] = CHSV(hue1, 255, 255);
        break;
        
      case 4:
        leds[33] = CHSV(hue1, 255, 255);
        leds[30] = CHSV(hue1, 255, 255);
        leds[28] = CHSV(hue1, 255, 255);
        leds[24] = CHSV(hue1, 255, 255);
        leds[169] = CHSV(hue1, 255, 255);
        leds[165] = CHSV(hue1, 255, 255);
        leds[163] = CHSV(hue1, 255, 255);
        leds[160] = CHSV(hue1, 255, 255);
        break;

      case 5:
        leds[29] = CHSV(hue1, 255, 255);
        leds[23] = CHSV(hue1, 255, 255);
        leds[170] = CHSV(hue1, 255, 255);
        leds[164] = CHSV(hue1, 255, 255);
        break;

      case 6:
        leds[37] = CHSV(hue1, 255, 255);
        leds[36] = CHSV(hue1, 255, 255);
        leds[35] = CHSV(hue1, 255, 255);
        leds[34] = CHSV(hue1, 255, 255);
        leds[159] = CHSV(hue1, 255, 255);
        leds[158] = CHSV(hue1, 255, 255);
        leds[157] = CHSV(hue1, 255, 255);
        leds[156] = CHSV(hue1, 255, 255);
        break;
  
      case 7:
        leds[38] = CHSV(hue1, 255, 255);
        leds[39] = CHSV(hue1, 255, 255);
        leds[40] = CHSV(hue1, 255, 255);
        leds[41] = CHSV(hue1, 255, 255);
        leds[22] = CHSV(hue1, 255, 255);
        leds[171] = CHSV(hue1, 255, 255);
        leds[152] = CHSV(hue1, 255, 255);
        leds[153] = CHSV(hue1, 255, 255);
        leds[154] = CHSV(hue1, 255, 255);
        leds[155] = CHSV(hue1, 255, 255);
        break;
  
      case 8:
        leds[51] = CHSV(hue1, 255, 255);
        leds[42] = CHSV(hue1, 255, 255);
        leds[21] = CHSV(hue1, 255, 255);
        leds[19] = CHSV(hue1, 255, 255);
        leds[18] = CHSV(hue1, 255, 255);
        leds[172] = CHSV(hue1, 255, 255);
        leds[151] = CHSV(hue1, 255, 255);
        leds[142] = CHSV(hue1, 255, 255);
        break;
 
      case 9:
        leds[52] = CHSV(hue1, 255, 255);
        leds[20] = CHSV(hue1, 255, 255);
        leds[17] = CHSV(hue1, 255, 255);
        leds[173] = CHSV(hue1, 255, 255);
        leds[141] = CHSV(hue1, 255, 255);
        break;
        
      case 10:
        leds[53] = CHSV(hue1, 255, 255);
        leds[50] = CHSV(hue1, 255, 255);
        leds[44] = CHSV(hue1, 255, 255);
        leds[43] = CHSV(hue1, 255, 255);
        leds[150] = CHSV(hue1, 255, 255);
        leds[149] = CHSV(hue1, 255, 255);
        leds[143] = CHSV(hue1, 255, 255);
        leds[140] = CHSV(hue1, 255, 255);
        break;

      case 11:
        leds[1] = CHSV(hue1, 255, 255);
        leds[0] = CHSV(hue1, 255, 255);
        leds[16] = CHSV(hue1, 255, 255);
        leds[15] = CHSV(hue1, 255, 255);
        break;
  
      case 12:
        leds[54] = CHSV(hue1, 255, 255);
        leds[49] = CHSV(hue1, 255, 255);
        leds[45] = CHSV(hue1, 255, 255);
        leds[2] = CHSV(hue1, 255, 255);
        leds[14] = CHSV(hue1, 255, 255);
        leds[148] = CHSV(hue1, 255, 255);
        leds[144] = CHSV(hue1, 255, 255);
        leds[139] = CHSV(hue1, 255, 255);
        break;

// TOp Center Row
      case 13:
        leds[57] = CHSV(hue1, 255, 255);
        leds[56] = CHSV(hue1, 255, 255);
        leds[55] = CHSV(hue1, 255, 255);
        leds[48] = CHSV(hue1, 255, 255);
        leds[47] = CHSV(hue1, 255, 255);
        leds[46] = CHSV(hue1, 255, 255);
        leds[3] = CHSV(hue1, 255, 255);
        leds[13] = CHSV(hue1, 255, 255);
        leds[147] = CHSV(hue1, 255, 255);
        leds[146] = CHSV(hue1, 255, 255);
        leds[145] = CHSV(hue1, 255, 255);
        leds[138] = CHSV(hue1, 255, 255);
        leds[137] = CHSV(hue1, 255, 255);
        leds[136] = CHSV(hue1, 255, 255);

        break;

// Bottom Center Row 
      case 14:
        leds[58] = CHSV(hue1, 255, 255);
        leds[59] = CHSV(hue1, 255, 255);
        leds[60] = CHSV(hue1, 255, 255);
        leds[67] = CHSV(hue1, 255, 255);
        leds[68] = CHSV(hue1, 255, 255);
        leds[69] = CHSV(hue1, 255, 255);
        leds[4] = CHSV(hue1, 255, 255);
        leds[12] = CHSV(hue1, 255, 255);
        leds[124] = CHSV(hue1, 255, 255);
        leds[125] = CHSV(hue1, 255, 255);
        leds[126] = CHSV(hue1, 255, 255);
        leds[133] = CHSV(hue1, 255, 255);
        leds[134] = CHSV(hue1, 255, 255);
        leds[135] = CHSV(hue1, 255, 255);
        break;

      case 15:
        leds[61] = CHSV(hue1, 255, 255);
        leds[66] = CHSV(hue1, 255, 255);
        leds[70] = CHSV(hue1, 255, 255);
        leds[5] = CHSV(hue1, 255, 255);
        leds[11] = CHSV(hue1, 255, 255);
        leds[123] = CHSV(hue1, 255, 255);
        leds[127] = CHSV(hue1, 255, 255);
        leds[132] = CHSV(hue1, 255, 255);
        break;
  
      case 16:
        leds[6] = CHSV(hue1, 255, 255);
        leds[7] = CHSV(hue1, 255, 255);
        leds[9] = CHSV(hue1, 255, 255);
        leds[10] = CHSV(hue1, 255, 255);
        break;

      case 17:
        leds[62] = CHSV(hue1, 255, 255);
        leds[65] = CHSV(hue1, 255, 255);
        leds[71] = CHSV(hue1, 255, 255);
        leds[72] = CHSV(hue1, 255, 255);
        leds[121] = CHSV(hue1, 255, 255);
        leds[122] = CHSV(hue1, 255, 255);
        leds[128] = CHSV(hue1, 255, 255);
        leds[131] = CHSV(hue1, 255, 255);
        break;

      case 18:
        leds[63] = CHSV(hue1, 255, 255);
        leds[95] = CHSV(hue1, 255, 255);
        leds[8] = CHSV(hue1, 255, 255);
        leds[98] = CHSV(hue1, 255, 255);
        leds[130] = CHSV(hue1, 255, 255);
        break;

      case 19:
        leds[64] = CHSV(hue1, 255, 255);
        leds[73] = CHSV(hue1, 255, 255);
        leds[94] = CHSV(hue1, 255, 255);
        leds[96] = CHSV(hue1, 255, 255);
        leds[97] = CHSV(hue1, 255, 255);
        leds[99] = CHSV(hue1, 255, 255);
        leds[120] = CHSV(hue1, 255, 255);
        leds[129] = CHSV(hue1, 255, 255);
        break;

      case 20:
        leds[77] = CHSV(hue1, 255, 255);
        leds[76] = CHSV(hue1, 255, 255);
        leds[75] = CHSV(hue1, 255, 255);
        leds[74] = CHSV(hue1, 255, 255);
        leds[93] = CHSV(hue1, 255, 255);
        leds[100] = CHSV(hue1, 255, 255);
        leds[119] = CHSV(hue1, 255, 255);
        leds[118] = CHSV(hue1, 255, 255);
        leds[117] = CHSV(hue1, 255, 255);
        leds[116] = CHSV(hue1, 255, 255);
        break;

      case 21:
        leds[78] = CHSV(hue1, 255, 255);
        leds[79] = CHSV(hue1, 255, 255);
        leds[80] = CHSV(hue1, 255, 255);
        leds[81] = CHSV(hue1, 255, 255);
        leds[112] = CHSV(hue1, 255, 255);
        leds[113] = CHSV(hue1, 255, 255);
        leds[114] = CHSV(hue1, 255, 255);
        leds[115] = CHSV(hue1, 255, 255);
        break;
      
      case 22:
        leds[86] = CHSV(hue1, 255, 255);
        leds[92] = CHSV(hue1, 255, 255);
        leds[101] = CHSV(hue1, 255, 255);
        leds[107] = CHSV(hue1, 255, 255);
        break;
        
      case 23:
        leds[82] = CHSV(hue1, 255, 255);
        leds[85] = CHSV(hue1, 255, 255);
        leds[87] = CHSV(hue1, 255, 255);
        leds[91] = CHSV(hue1, 255, 255);
        leds[102] = CHSV(hue1, 255, 255);
        leds[106] = CHSV(hue1, 255, 255);
        leds[108] = CHSV(hue1, 255, 255);
        leds[111] = CHSV(hue1, 255, 255);
        break;

      case 24:
        leds[83] = CHSV(hue1, 255, 255);
        leds[88] = CHSV(hue1, 255, 255);
        leds[105] = CHSV(hue1, 255, 255);
        leds[110] = CHSV(hue1, 255, 255);
        break;
     
      case 25:
        leds[84] = CHSV(hue1, 255, 255);
        leds[90] = CHSV(hue1, 255, 255);
        leds[103] = CHSV(hue1, 255, 255);
        leds[109] = CHSV(hue1, 255, 255);
        break;
      
      case 26:
        leds[89] = CHSV(hue1, 255, 255);
        leds[104] = CHSV(hue1, 255, 255);
        break;
    
      default:
        // if nothing else matches, do the default
        // HOLD it solid for a few cycles... maybe later we can add some animation.  
        // Now we'll hold it solid for cycles 11-20
        break;
      }  
  
      FastLED.show();
      //delay (20);
      delay (30);
      
      //fadeToBlackBy( leds, NUM_LEDS, 25);
      fadeToBlackBy( leds, NUM_LEDS, 50);
    }
    // end display loop 
  }
  // end loop for number of displays loop
}



void branch_fade()
{
  uint8_t branch_id = 1;
  uint8_t TOTAL_CYCLES = 60;

  uint8_t offset_br1;
  uint8_t offset_br2;
  uint8_t offset_br3;
  uint8_t offset_br4;
  uint8_t offset_br5;
  uint8_t offset_br6;

  // random color
  uint8_t hue1 = random8(255);

  for (int j=0; j<5; j++) {
    for (int current_cycle=0; current_cycle < TOTAL_CYCLES; current_cycle++) {
  
      int branch_id;
      branch_id = current_cycle % 10;

      // I changed the order to 50, 40, 30, .. 0 so that this goes in reverse.  All other
      // effects go clockwise.  I wanted something that went the other direction!
      if (current_cycle == 50) {
        for (int i=0; i<26; i++) {
          uint8_t offset_br1 = pgm_read_byte(&flakeBranch1Array[i]);
          leds[offset_br1] = CHSV(hue1, 255, 255);    
        }
      }
  
      if (current_cycle == 40) {
        for (int i=0; i<26; i++) {
          uint8_t offset_br2 = pgm_read_byte(&flakeBranch2Array[i]);
          leds[offset_br2] = CHSV(hue1, 255, 255);    
        }
      }
  
      if (current_cycle == 30) {
        for (int i=0; i<26; i++) {
          uint8_t offset_br3 = pgm_read_byte(&flakeBranch3Array[i]);
          leds[offset_br3] = CHSV(hue1, 255, 255);    
        }
      }
  
      if (current_cycle == 20) {
        for (int i=0; i<26; i++) {
          uint8_t offset_br4 = pgm_read_byte(&flakeBranch4Array[i]);
          leds[offset_br4] = CHSV(hue1, 255, 255);    
        }
      }
      
      if (current_cycle == 10) {
        for (int i=0; i<26; i++) {
          uint8_t offset_br5 = pgm_read_byte(&flakeBranch5Array[i]);
          leds[offset_br5] = CHSV(hue1, 255, 255);    
        }
      }
  
      if (current_cycle == 0) {
        for (int i=0; i<26; i++) {
          uint8_t offset_br6 = pgm_read_byte(&flakeBranch6Array[i]);
          leds[offset_br6] = CHSV(hue1, 255, 255);    
        }
      }
  
      FastLED.show();
      delay(30);
      fadeToBlackBy( leds, NUM_LEDS, 25);
    }
    // end of display control
  }
  // end of total cycles loop

}



// turn on all LEDs are differnt colors - flash them to get attention, then fade out
void fulldisplay_w_flash() 
{

  // start with 3 different colors - if more or less required, adjust the if statement below
  // 300 ms on, 100 ms off seems pretty striking
  for (int color_id=0; color_id<3; color_id++) {

    for (int i=0; i<6; i++) {
      FastLED.clear();
      FastLED.show();
      delay (150);    

      // pick a color based on color ID
      if (color_id == 0) { fill_solid( leds, NUM_LEDS, CRGB::White ); }
      if (color_id == 1) { fill_solid( leds, NUM_LEDS, CRGB::Red );   }
      if (color_id == 2) { fill_solid( leds, NUM_LEDS, CRGB::Green);  }
      
      FastLED.show();
      delay (300);    
    }
  
    // all LEDs are now on, fade to black
    for (int j=0; j<20; j++) {
      fadeToBlackBy( leds, NUM_LEDS, 75);
      FastLED.show();
      delay(35);
    }
    
  }
  // end of color loops
}


void fill_solid_color() 
{ 
  
  uint8_t hue1 = random8(255);
  uint8_t hue2 = random8(255);
   
  for (int i=0; i<NUM_LEDS; i++) {
    Serial.println (i);

    leds[i] = CHSV( hue1, 255, 255);
    leds[i-1] = CHSV( hue2, 255, 255);
    FastLED.show();
    leds[i] = CRGB::Black;

    //delay(30);    
    delay(20);    
  }
}

void fill_black()
{
  FastLED.clear();
  //fill_solid(leds, NUM_LEDS, CRGB::Black);
}


void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}


void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}


void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


// draws a line that fades between 2 random colors
// TODO:  Add logic to rotate the starting point
void gradient_fill() {

//  uint8_t hue1 = 60;
//  uint8_t hue2 = random8(255);
  uint8_t hue1 = random8(255);
  uint8_t hue2 = hue1 + random8(30,61);
  
  for( int i = 0; i < NUM_LEDS; i++){
    //fill_gradient (leds, 0, CHSV(0, 255, 255), i, CHSV(96, 255, 255), SHORTEST_HUES);
    fill_gradient (leds, 0, CHSV(hue1, 255, 255), i, CHSV(hue2, 255, 255), SHORTEST_HUES);
    delay(25);
    FastLED.show();
    //FastLED.clear();
  }
}

// draws a line that fades between 2 random colors
// Added logic to rotate the starting corner
/*
void gradient_fill_2() {

  uint8_t hue1 = random8(255);
  uint8_t hue2 = hue1 + random8(30,61);
  uint8_t offset2;
  CRGB grad[NUM_LEDS];  // A place to save the gradient colors. (Don't edit this)
  fill_gradient (grad, 0, CHSV(hue1, 255, 255), NUM_LEDS, CHSV(hue2, 255, 255), SHORTEST_HUES);

  // display each pixel one at a time
  for( int i = 0; i < NUM_LEDS; i++){

    if ( (i+offset_for_star_rotation) < NUM_LEDS ) {
      offset2 = i+offset_for_star_rotation;
    }
    else {
      offset2 = i+offset_for_star_rotation-NUM_LEDS;
    }
    
    leds[offset2] = grad[i];
    //delay(25);
    delay(12);
    FastLED.show();
  }
  
  // rotate star starting point for next loop
  offset_for_star_rotation += 20;
  if (offset_for_star_rotation >= 100) {
    offset_for_star_rotation = 0;
  }
  FastLED.clear();
}
*/



// Adapted from code by Marc Miller.  Original Header:
//
//***************************************************************
// Marquee fun (v3)
//  Pixel position down the strip comes from this formula:
//      pos = spacing * (i-1) + spacing
//  i starts at 0 and is incremented by +1 up to NUM_LEDS/spacing.
//
// Marc Miller, May 2016
//***************************************************************
void marque_v3() {

    
  for (uint8_t i=0; i<(NUM_LEDS/spacing); i++){
    for (uint8_t w = 0; w<width; w++){
      pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
      if ( w % 2== 0 ){  // Is w even or odd?
        color = hue;
      } else {
        color = hue + hue2_shift;
      }
      
      leds[pos] = CHSV(color,255,255);
    }
    
    if (DEBUG==1) {  // Print out lit pixels if DEBUG is true.
      Serial.print(" "); Serial.print(pos);
    }
    delay(10);
  }
  if (DEBUG==1) { Serial.println(" "); }
  FastLED.show();
  
  // Fade out tail or set back to black for next loop around.
  if (fadingTail == 1) {
    fadeToBlackBy(leds, NUM_LEDS,fadeRate);
  } else {
    for (uint8_t i=0; i<(NUM_LEDS/spacing); i++){
      for (uint8_t w = 0; w<width; w++){
        pos = (spacing * (i-1) + spacing + advance + w) % NUM_LEDS;
        leds[pos] = CRGB::Black;
      }
    }
  }

  // Advance pixel postion down strip, and rollover if needed.
  advance = (advance + delta + NUM_LEDS) % NUM_LEDS;
}


void TestGlitter() 
{
  FastLED.clear();
  addGlitter(95);
  delay(25);
}

  
// add several random white spots/stars/glitters
void addGlitter( fract8 chanceOfGlitter) 
{
  for (int i=0; i<20; i++) {
    if( random8() < chanceOfGlitter) {
      leds[ random16(NUM_LEDS) ] += CRGB::White;  
    }
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}


// END
