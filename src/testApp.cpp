//
//  Poem Tweets
//  
//  Built using OpenFrameworks
//
//  Created by Axel Roest on 12-03-11.
//  Copyright 2011 Axel Dingen bv. All rights reserved.
//
//  Nifty fontcaching routine (ofxFontAssets) courtesy of Rick Companje

#include "testApp.h"
// necessary for zin-splitting
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>


//--------------------------------------------------------------
/* The road to wisdom? — Well, it's plain
 and simple to express:
 Err
 and err
 and err again
 but less
 and less
 and less.

 Grook van Piet Hein
*/
/* ToDo:
 split up simple string with newlines into separate words 
 */

void testApp::setup(){
  ofBackground(255, 255, 230);

  // NEW
//  string poem="The road to wisdom? Well it's plain\nand simple to express:\nErr\nand err\nand err again\nbut less\nand less\nand less.";
	string poem="How many masks wear we, and undermasks,\nUpon our countenance of soul, and when,\nIf for self-sport the soul itself unmasks,\nKnows it the last mask off and the face plain?\nThe true mask feels no inside to the mask\nBut looks out of the mask by co-masked eyes.\nWhatever consciousness begins the task\nThe task’s accepted use to sleepness ties.\nLike a child frighted by its mirrored faces,\nOur souls, that children are, being thought-losing,\nFoist otherness upon their seen grimaces\nAnd get a whole world on their seen grimaces\nAnd get a whole world on their forgot causing;\nAnd, when a thought would unmask our soul’s masking,\nItself goes not unmasked to the unmasking.";
  extractWordsFromPoem(poem);
  
  ofEnableAlphaBlending();
  ofSetFrameRate(30);
  selectedWord = -1;
  
  // test
  wordFont.loadFont(FONTREGULAR, TEXTSIZE);
  boldFont.loadFont(FONTBOLD, TEXTBOLDSIZE);
  frFont.loadFont(TWEETFONT, 24);           // to display framerate
 
  lastWordSelected=0;
  twitter.setup();
	twitter.setSearchDelegate(this);
  
  // initialise fontCache
/*  for (int i=1 ; i<256 ; i++) {
    fontCache[i]=&0;
  }
 */
}

//--------------------------------------------------------------
void testApp::update()
{
  // update animation if we have tweetanimations
  if (tweetanimations.size() > 0) {
    for (int i=(tweetanimations.size()-1) ; i >= 0  ; i--) {
      if (updateTweet(&tweetanimations[i])) {
        // remove this tweet (which lowers the tweetanimations.size by one, that's why we go 'backwards'
        tweetanimations.erase(tweetanimations.begin() + i);
      }
    }
  }
  
}

//--------------------------------------------------------------
void testApp::draw(){
  // NEW way to draw poem
  
  for (int i=0 ; i<words.size() ; i++) {
    drawWord(words[i],selectedWord == i);
  }
  
  // Draw tweetanimations (if they're there)
  if (tweetanimations.size() > 0) {
    for (int i=0 ; i < tweetanimations.size() ; i++) {
      drawTweet(tweetanimations[i]);
    }
  }
  
  // display framerate
  char buffer[32];
  int result;
  result = snprintf(buffer, 32, "%4f", ofGetFrameRate());
  
  ofSetColor(120, 120, 120);
  frFont.drawString(buffer, 10, 30);
  
  // draw mouse (text position debugging)
/*  ofSetColor(0x00FFFF);
  wordFont.drawString("Muis", mouseX, mouseY);
  ofRectangle rect = wordFont.getStringBoundingBox("Bounds", mouseX, mouseY);
  wordFont.drawString("Bounds", rect.x, rect.y+rect.height);
 */
}

#pragma mark events
//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{
  ofPoint mousePtr;
  selectedWord = -1;
  
  for (int i=0 ; i<words.size() ; i++) {
    mousePtr.x=x;
    mousePtr.y=y;
 //   cout << i << "\t" << x << "," << y << "\t[ " << words[i].bounds.x << "," << words[i].bounds.y << " ]\n";
    if (pointInRect(mousePtr, words[i].bounds)) {
      // muis beweegt over woord
      selectedWord = i;
      i = words.size();       // stop messing about in the loop
    } 
  }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
// we might want to do Twitter lookups here later on
  if (selectedWord >= 0) {      // we are hovering over a word
        // Unfortunately, we cannot misuse the selectedWord for the twitterpage.
    twitter.startTwitterQuery(words[selectedWord].text, 10, 1);
    lastWordSelected = selectedWord;      // no queueing, just one click at a time!
  } 
  
  /* OUD
  if (selectedWord >= 0) {      // we are hovering over a word
    for (int i=0 ; i < NUMTWEETS ; i++) {   // fill array with text so far.
      createTweet(words[selectedWord], words[selectedWord].text);
    }
  }
   */
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

#pragma mark -
#pragma mark tools

bool testApp::pointInRect(ofPoint point, ofRectangle rect)
{
  if (point.x > rect.x && point.x < (rect.x + rect.width) && 
      point.y > rect.y && point.y < (rect.y + rect.height)) {
    return true;
  } else {
    return false;
  }
}

/* N.B. the bounding box of a string has the position in the top-left corner
 whereas DRAWING the string starts in the lower-left corner. 
 Great.
 */

void testApp::createTweet(AnimText origWord, string text)
{
  AnimText word;
  
  word.text = text;
  // start tweetanimations in the middle of the original poem word
  word.bounds.x = origWord.bounds.x + (origWord.bounds.width * 0.5);
  word.bounds.y = origWord.bounds.y + (origWord.bounds.height * 0.5);
  word.line = 0;
  word.word = 0;
  word.rotation = ofRandom(-40,40);
  word.vx = ofRandom(-10,10);
  word.vy = ofRandom(-10,10);
  word.alpha = 255;
  word.fontsize = 1;
  
  tweetanimations.push_back(word);
}

int testApp::createWord(string myWord,int line, int wordnumber, int startposition)
{
  AnimText word;
  ofTrueTypeFont wordFont;
  
  word.text = myWord;
  word.line = line;
  word.word = wordnumber;
  word.rotation = 0.0;
  word.vx = word.vy = 0;
  word.alpha = 255;
  
  // get word size and formatted text
  wordFont.loadFont(FONTREGULAR, TEXTSIZE);
  word.bounds = wordFont.getStringBoundingBox(myWord, startposition,POEMOFFSETY + line * LINESIZE);
  int woordbreedte = word.bounds.width;

  words.push_back(word);
  
  return (startposition + woordbreedte + SPACE);
}

void testApp::extractWordsFromSentences(string lines[])
// split up sentences in words
// NOT USED ANYMORE
{
  string  zin;
  
  for (int i = 0 ; i < NRSTRINGS ; i++) {
    int offsetx=POEMOFFSETX;
    int     n=0;
    zin = lines[i];
    // cout << zin << endl;
    istringstream iss(lines[i]);
    do
    {
      string sub;
      iss >> sub;
      if (sub.length() > 0) {
        // cout << "Substring: " << sub << endl;
        offsetx = createWord(sub,i,n,offsetx);
      }
      n++;
    } while (iss);
  }
}

void testApp::extractWordsFromPoem(string poem)
// split up sentences in words
{
  int i=0;
  char buffer[512];
  
  istringstream lines(poem);
  do {
    string  zin;
    int offsetx=POEMOFFSETX;
    int     n=0;
    
    lines.getline(buffer, 511, '\n');          // split up poem in lines
    zin = buffer;
    
    // cout << zin << endl;
    istringstream iss(zin);                    // split up line in words
    do {
      string sub;
      iss >> sub;
      if (sub.length() > 0) {
      //  cout << "Substring: " << sub << endl;
        offsetx = createWord(sub,i,n,offsetx);
      }
      n++;
    } while (iss);
    i++;
  } while (lines);
}

string testApp::makeFontNameSize(string fontName, int fontSize)
{
  string s;
  stringstream out;
  // method 1
/*  out << fontSize;
  s = fontName + "_" + out.str();
*/  // method 2
  out << fontName << "_" << fontSize;
  s = out.str();
  return s;
}

#pragma mark -
void testApp::drawWord(AnimText word, bool selected)
{
//  ofTrueTypeFont wordFont, boldFont;
  
  ofSetColor(0x0000FF);
  if (selected) {
    ofSetColor(0x7F00FF);
    // because the bold size is bigger, center it over the original text
    int newWidth = boldFont.stringWidth(word.text);
    int newHeight = boldFont.stringHeight(word.text);
    int x = word.bounds.x - (newWidth - word.bounds.width)* 0.5;
    int y = word.bounds.y - (newHeight - word.bounds.height)* 0.5;
    boldFont.drawString(word.text, x, y+newHeight); 
  } else {
    wordFont.drawString(word.text, word.bounds.x,word.bounds.y+word.bounds.height);      
  }
  
}
// http://www.daniweb.com/software-development/cpp/threads/173285
void testApp::drawTweet(AnimText tweet)
{
  ofSetColor(TWEETCOLOUR,tweet.alpha);

  string fontns = makeFontNameSize(TWEETFONT, tweet.fontsize);
  
  ofTrueTypeFont *tweetFont = &fontCache[fontns];         // copy constructor used???
  ofPushMatrix();
    ofRotate(tweet.rotation, tweet.bounds.x, tweet.bounds.y, 1);

    tweetFont->drawString(tweet.text, tweet.bounds.x,tweet.bounds.y);      
	ofPopMatrix();

//  ofTrueTypeFont tweetFont;
//  tweetFont.loadFont(TWEETFONT, tweet.fontsize);
//  tweetFont.drawString(tweet.text, tweet.bounds.x,tweet.bounds.y);  
}

bool testApp::updateTweet(AnimText *tweet)
// if it returns true, alpha has gone negative and we can remove the tweet from the vector
{
  // update position
  tweet->bounds.x += tweet->vx;
  tweet->bounds.y += tweet->vy;
  
  tweet->fontsize += 1;          // to fast growing? ---> use float
  tweet->alpha-=5;
  
  return (tweet->alpha<0);
}

#pragma mark -
#pragma mark twitter delegate function
void testApp::searchResult(vector<Tweet> results) {
	
	tweets.clear();           // store new tweets here, maybe not: we'll see later when it works
	
	for(int i=0; i<results.size(); i++) {
    if (lastWordSelected >= 0) {      // we are hovering over a word
      createTweet(words[lastWordSelected], results[i].title);
    }
	}
}
