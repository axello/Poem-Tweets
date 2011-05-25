//
//  Poem Tweets
//  
//
//  Created by Axel Roest on 12-03-11.
//  Copyright 2011 Axel Dingen bv. All rights reserved.
//

#ifndef _TEST_APP
#define _TEST_APP

#define NRSTRINGS 8
#define MAXANIM 20

// #define AXEL
#define FERNANDA

// poem text constants
#ifdef AXEL
#define SPACE 20
#define LINESIZE 80
#define TEXTSIZE 32
#define TEXTBOLDSIZE 36
#define POEMOFFSETX 100
#define POEMOFFSETY 100
#define FONTREGULAR "Vera.ttf"
#define FONTBOLD    "VeraBd.ttf"

#define NUMTWEETS 20
#define TWEETFONT   "Vera.ttf"
#define TWEETCOLOUR 40,40,80
#endif

// poem text constants
#ifdef FERNANDA
#define SPACE 12
#define LINESIZE 45
#define TEXTSIZE 15
#define TEXTBOLDSIZE 22
#define POEMOFFSETX 250
#define POEMOFFSETY 75
#define FONTREGULAR "Vera.ttf"
#define FONTBOLD    "VeraBd.ttf"

#define NUMTWEETS 10
#define TWEETFONT   "Vera.ttf"
#define TWEETCOLOUR 40,40,80
#endif


#include "ofMain.h"
#include "ofxFontAssets.h"
#include "ofxTwitter.h"

struct AnimText  {
  ofRectangle bounds;       // position and size of bounding box of word
  int vx;           // velocity (used for animation)
  int vy;
  float rotation;
  int alpha;
  int line;         // line in poem
  int word;         // word in line
  int tag;          // we can use this to group tweetanimations together, e.g. for a colour
  int fontsize;     // for now, later use graphics scaling for smoother animation
  string text;      // the text, a sentence or a word
};

class testApp : public ofBaseApp, TwitterDelegate {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
  
	void searchResult(vector<Tweet> results);
  vector<AnimText> words;
  vector<AnimText> tweetanimations;
  
	ofxTwitter twitter;
  vector<Tweet> tweets;
  int lastWordSelected;
  
  ofxFontAssets         fontCache;
//  map<int,ofTrueTypeFont> fontCache;       // because fontloading is so sloooooooow

private:
  int  createWord(string myWord,int line, int wordnumber, int startposition);
  void createTweet(AnimText origWord, string text);
  void extractWordsFromSentences(string lines[]);
  void extractWordsFromPoem(string poem);
  bool pointInRect(ofPoint point, ofRectangle rect);
  void drawWord(AnimText word, bool selected);
  void drawTweet(AnimText tweet);
  bool updateTweet(AnimText *tweet);
  string makeFontNameSize(string fontName, int fontSize);

  int regel;
  int selectedWord;
  ofTrueTypeFont wordFont, boldFont;          // is this the cause of the memory leak?
  ofTrueTypeFont  frFont;
  

};

#endif
