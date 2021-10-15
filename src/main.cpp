#include <Arduino.h>
// Lord Vetinari also has a strange clock in his waiting-room. While it does keep completely accurate time overall, it sometimes ticks and tocks out of sync (example: "tick, tock ... ticktocktick, tock ...") 
// and occasionally misses a tick or tock altogether, which has the net effect of turning one's brain "into a sort of porridge". (Feet of Clay, Going Postal). In Feet of Clay Vimes observes that it must have cost him quite a lot of money.

// Someone very clever - certainly someone much cleverer than whoever had trained that imp - must have made the clock for the Patrician's waiting room. It went tick-tock like any other clock. But somehow, and against all usual horological practice, the tick and the tock were irregular. Tick tock tick . . . and then the merest fraction of a second longer before. . . tock tick tock. . . and then a tick a fraction of a second earlier than the mind's ear was now prepared for. The effect was enough, after ten minutes, to reduce the thinking processes of even the best-prepared to a sort of porridge. The Patrician must have paid the clockmaker quite highly.
#include <WiFiManager.h>  
// NTP client om de tijd op te halen (hier krijg je Time.h bij)
#include <NtpClientLib.h>

const char* SSID = "vetinariclock";

boolean tock = false;

long ticks = 0;

void setup() {
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  digitalWrite(D7, LOW);
  digitalWrite(D8, LOW);
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.setTimeout(120);
  wifiManager.autoConnect(SSID); 

  NTP.begin ("pool.ntp.org", 1, true, 0);
  NTP.setInterval(10,3600);
}



void tick(int t) {

  if (tock) {
    digitalWrite(D7, HIGH);
    delay(30);
    digitalWrite(D7, LOW);
    delay(t-30);
  } else {
    digitalWrite(D8, HIGH);
    delay(30);
    digitalWrite(D8, LOW);
    delay(t-30);
  }

  tock = !tock;
  ticks++;
}

void catchup() {
  Serial.println("catching up");
  // while (ticks < (millis() / 1000)) {
  while (ticks < NTP.getUptime()) {
    tick(250);
  }
  // while (ticks > (millis() / 1000)) {
  while (ticks > NTP.getUptime()) {
    delay(10);
  }
}

void skipannoy() {
  Serial.println("annoy");
  delay(1500);
  tick(250);
  tick(250);
}

void loop() {
  
  if (random(60)>5) {
    tick(900+random(200));
  } else {
    skipannoy();
  }

  if (random(60)<2) {
    Serial.println("desync");
    delay(500); // desync
  }

  if (random(60)>58) {
    catchup();
  }
    unsigned long secs = millis() / 1000;
    Serial.print(secs);
    Serial.print(" ");
    Serial.println(ticks);

  time_t ntpTime = now();
  Serial.println(ntpTime);
  Serial.println(NTP.getUptime());
  
}