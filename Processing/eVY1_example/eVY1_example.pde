import themidibus.*;

MidiBus vocaloid;
int channel = 0; //0:eVocaloid
int pitch = 0;
int velocity = 0;

void setup() {
  size(800, 300);
  background(255);
  MidiBus.list();
  vocaloid = new MidiBus(this, "eVY1 MIDI" , "eVY1 MIDI");
}

void draw() {
  if(keyPressed && key == ' ') {
    pitch = int(128 * mouseX / width + 0.5);
    velocity = int(128 * mouseY / height + 0.5);
    vocaloid.sendNoteOn(channel, pitch, velocity);
  } else {
    vocaloid.sendNoteOff(channel, pitch, velocity);
  }
  
  background(60);
  
  // draw selected block
  int block = int(128*mouseX/width+0.5);
  int red = int(128*mouseY/height+0.5);
  fill(50+red, 40, 40);
  rect(block*width/128, 0, width/128, height);
  for(int x = 0; x < 128; x++) {
    stroke(0);
    line(x*width/128, 0, x*width/128, height);
  }
  
  // draw info
  fill(255);
  text("pitch ->", 15, 10);
  pushMatrix();
    rotate(HALF_PI);
    text("velocity ->", 15, -5);
  popMatrix();
  
  // update title bar
  String title;
  title = "pitch:" + str(block) + " velocity" + str(red);
  frame.setTitle(title);
}

void exit() {
  vocaloid.sendNoteOff(channel, pitch, velocity);
  vocaloid.close(); // bye bye!
  super.exit();
}